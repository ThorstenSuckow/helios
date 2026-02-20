// ============================================================================
// Includes
// ============================================================================
#include <algorithm>
#include <cstdlib>
#include <glad/gl.h>
#include <iostream>
#include <numbers>

// ============================================================================
// Module Imports
// ============================================================================
import helios;
import helios.ext;
import helios.examples.spaceshipControl.SpaceshipWidget;

// ============================================================================
// Using Declarations
// ============================================================================
using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::mesh;
using namespace helios::rendering::material;
using namespace helios::rendering::shader;
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::rendering::asset::shape::basic;
using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::input::types;
using namespace helios::ext::glfw::window;
using namespace helios::util::io;
using namespace helios::scene;
using namespace helios::math;
using namespace helios::engine::state;

// ============================================================================
// Entry Point
// ============================================================================
int main() {

    // ========================================
    // 1. Constants
    // ========================================
    constexpr float CELL_SIZE          = 5.0f;
    constexpr float SPACESHIP_SIZE     = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    // ========================================
    // 2. Application and Window Setup
    // ========================================
    helios::engine::bootstrap::registerAllComponents();

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Spaceship Control", 1980, 1024, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto sceneToViewportMap = helios::engine::modules::scene::types::SceneToViewportMap();
    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f,
        helios::engine::core::data::ViewportId{"mainViewport"});

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    helios::input::InputManager& inputManager = app->inputManager();
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);

    const auto basicStringFileReader = BasicStringFileReader();

    // ----------------------------------------
    // 2.1 ImGui and Debug Tooling
    // ----------------------------------------
    auto imguiBackend = helios::ext::imgui::ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = helios::ext::imgui::ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics = helios::engine::tooling::FpsMetrics();
    auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
    auto framePacer = helios::engine::tooling::FramePacer(std::move(stopwatch));
    framePacer.setTargetFps(0.0f);
    helios::engine::tooling::FrameStats frameStats{};
    auto menu = new helios::ext::imgui::widgets::MainMenuWidget();
    auto fpsWidget = new helios::ext::imgui::widgets::FpsWidget(&fpsMetrics, &framePacer);
    auto gamepadWidget = new helios::ext::imgui::widgets::GamepadWidget(&inputManager);
    auto logWidget = new helios::ext::imgui::widgets::LogWidget();
    auto cameraWidget = new helios::ext::imgui::widgets::CameraWidget();
    auto spaceshipWidget = new helios::examples::spaceshipControl::SpaceshipWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(gamepadWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);
    imguiOverlay.addWidget(spaceshipWidget);

    // ----------------------------------------
    // 2.2 Logger Configuration
    // ----------------------------------------
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 3. Shader Creation
    // ========================================
    auto shader_ptr =
            std::make_shared<OpenGLShader>("./resources/cube.vert", "./resources/cube.frag", basicStringFileReader);

    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    shader_ptr->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 4. Scene Graph and Camera Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(
        std::move(frustumCullingStrategy), helios::engine::core::data::SceneId{"mainScene"});
    sceneToViewportMap.add(scene.get(), mainViewport.get());

    auto mainViewportCam = std::make_unique<helios::scene::Camera>();
    auto cameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();

    cameraSceneNode_ptr->setInheritance(
        helios::math::TransformType::Translation
    );
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(
        helios::math::vec3f(0.0f, 0.0f, -100.0f)
    );
    cameraSceneNode_ptr->camera().setPerspective(
        FOVY,
        ASPECT_RATIO_NUMER / ASPECT_RATIO_DENOM,
        0.1f,
        1000.0f
    );
    cameraSceneNode_ptr->lookAtLocal(
        vec3f(0.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f)
    );
    cameraWidget->addCameraSceneNode("Main Camera", cameraSceneNode_ptr);

    // ========================================
    // 5. GameWorld and Level Setup
    // ========================================
    helios::engine::runtime::gameloop::GameLoop gameLoop{};
    helios::engine::runtime::world::GameWorld gameWorld{};

    gameWorld.session().trackState<helios::engine::mechanics::gamestate::types::GameState>();
    gameWorld.session().trackState<helios::engine::mechanics::match::types::MatchState>();

    auto level = std::make_unique<helios::engine::runtime::world::Level>(&(scene.get()->root()));
    auto* levelPtr = level.get();
    level->setBounds(
        helios::math::aabb{
            -(GRID_X * CELL_SIZE)/2.0f, -(GRID_Y * CELL_SIZE)/2.0f, 0.0f,
            (GRID_X * CELL_SIZE)/2.0f, (GRID_Y * CELL_SIZE)/2.0f, 0.0f
        },
        helios::core::units::Unit::Meter
    );
    gameWorld.setLevel(std::move(level));

    // ========================================
    // 6. GameObjects
    // ========================================

    // Grid
    auto theGrid = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&shader_ptr, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(shader_ptr)
               .color(helios::util::Colors::Turquoise.withW(0.2f))
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Grid>(29, 19))
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .translate(helios::math::vec3f(0.0f, 0.0f, 0.5f))
              .scale(helios::math::vec3f(GRID_X * CELL_SIZE, GRID_Y * CELL_SIZE, 0.0f));
        })
        .make();

    // Player ship
    auto shipGameObject = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&shader_ptr, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(shader_ptr)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_SIZE, SPACESHIP_SIZE, 0.0f));
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(30.0f)
               .instantAcceleration(false);
            mcb.steering()
               .steeringSetsDirection(true)
               .instantSteering(false);
        })
        .make();

    // Debug gizmos (input visualization)
    auto leftStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&shader_ptr, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(shader_ptr)
               .color(helios::util::Colors::White)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        }).make();

    auto shipDirectionGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&shader_ptr, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(shader_ptr)
               .color(helios::util::Colors::Red)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    // ========================================
    // 7. State-to-Viewport Mapping
    // ========================================
    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::core::data;

    auto stateToViewportMap = helios::engine::state::StateToIdMapPair<
        GameState, MatchState, ViewportId
    >();
    stateToViewportMap.add(GameState::Any, ViewportId{"mainViewport"});
    stateToViewportMap.freeze();

    // ========================================
    // 8. GameLoop Phase Configuration
    // ========================================
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass<GameState>(GameState::Any)
             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()
             .addSystem<
                 helios::engine::modules::rendering::viewport::systems::StateToViewportPolicyUpdateSystem
                    <GameState, MatchState>>(stateToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneSyncSystem>(sceneToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneRenderingSystem>(
                 app->renderingDevice(), sceneToViewportMap)
             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>();

    // ========================================
    // 9. Initialization and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    leftStickGizmo.setActive(true);
    shipDirectionGizmo.setActive(true);
    theGrid.setActive(true);

    std::ignore = shipGameObject.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode()->addNode(std::move(cameraSceneNode));

    shipGameObject.setActive(true);

    auto* leftStickGizmoNode = leftStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    spaceshipWidget->addGameObject("Player 1", shipGameObject);

    gameWorld.init();
    gameLoop.init(gameWorld);

    gameWorld.session().setStateFrom<GameState>(
        StateTransitionContext<GameState>(GameState::Undefined, GameState::Start, GameStateTransitionId::StartRequested)
    );

    bool showImgui = true;
    bool tilde = false;

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // 9.1 Event and Input Processing
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC] - Exiting..." << std::endl;
            win->setShouldClose(true);
        }

        if (!tilde && inputManager.isKeyPressed(Key::TILDE)) {
            tilde = true;
            showImgui = !showImgui;
        }
        if (tilde && inputManager.isKeyReleased(Key::TILDE)) {
            tilde = false;
        }

        // 9.2 Game Logic Update
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);

        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);

        // 9.3 Debug Gizmo Update
        const auto* mc = shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mc) {
            leftStickGizmoNode->setScale((mc->direction() * mc->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mc->velocity().normalize() * mc->speedRatio() * 4.0f);
        }

        // 9.4 ImGui Overlay
        if (showImgui) {
            imguiOverlay.render();
        }

        // 9.5 Frame Synchronization
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
