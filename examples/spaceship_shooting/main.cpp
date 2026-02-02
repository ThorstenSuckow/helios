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
import helios.examples.spaceshipShooting.SpaceshipWidget;


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


// ============================================================================
// Entry Point
// ============================================================================

int main() {

    // ========================================
    // Constants
    // ========================================
    constexpr float CELL_SIZE          = 5.0f;
    constexpr float SPACESHIP_SIZE     = 5.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;



    // ========================================
    // 1. Application and Window Setup
    // ========================================

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Spaceship Shooting Demo", 1980, 1024, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    // Get the InputManager for handling keyboard input
    helios::input::InputManager& inputManager = app->inputManager();
    // register the gamepads
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);

    const auto basicStringFileReader = BasicStringFileReader();

    // ========================================
    // 1.2. ImGui & Tooling Setup
    // ========================================
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
    auto spaceshipWidget = new helios::examples::spaceshipShooting::SpaceshipWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(gamepadWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);
    imguiOverlay.addWidget(spaceshipWidget);

    // ========================================
    // 1.3 Logger Configuration
    // ========================================
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 2. Shader Creation
    // ========================================
    auto defaultShader =
            std::make_shared<OpenGLShader>("./resources/cube.vert", "./resources/cube.frag", basicStringFileReader);

    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    defaultShader->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 3. Scene Graph Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));

    // ========================================
    // 4. Camera Setup
    // ========================================
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
    // 5. GameWorld, Level and GameObjects
    // ========================================

    helios::engine::runtime::gameloop::GameLoop gameLoop{};
    helios::engine::runtime::world::GameWorld gameWorld{};

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

    // projectile prefab
    auto projectilePrefab = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Ellipse>(0.5f, 0.2f, 8))
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(2.2f, 0.8f, 0.0f), helios::core::units::Unit::Meter);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(80.0f)
               .instantAcceleration(true);
            mcb.steering()
               .steeringSetsDirection(false)
               .instantSteering(true);
        })
        .withSpawn([](auto& sb) {
             sb.spawn()
               .useSpawnProfile();
         })
        .make();

    // THE GRID
    auto theGrid = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Turquoise.withW(0.2f))
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Grid>(29, 19))
               .attachTo(&root);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .translate(helios::math::vec3f(0.0f, 0.0f, 0.5f))
              .scale(helios::math::vec3f(GRID_X * CELL_SIZE, GRID_Y * CELL_SIZE, 0.0f));
        })
        .make();

    // ship game object
    auto shipGameObject = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(helios::math::vec3f(SPACESHIP_SIZE, SPACESHIP_SIZE, 0.0f));
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce);
        })
        .withShooting([](auto& ccb) {
            ccb.weapon()
               .fireRate(5.0f);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(30.0f)
               .instantAcceleration(false);
            mcb.steering()
               .steeringSetsDirection(true)
               .instantSteering(false);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .make();

    // GIZMO Left stick
    auto leftStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::White)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        }).make();

    auto rightStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Magenta)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    auto shipDirectionGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject()
        .withRendering([&defaultShader, &shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Red)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject.get())
               .inherit(helios::math::TransformType::Translation);
        })
        .make();


    using namespace helios::engine::runtime::spawn;

    // --------------------------
    //     SPAWN CONFIGURATION
    // --------------------------

    constexpr helios::engine::core::data::GameObjectPoolId ProjectilePoolId{"projectile_pool"};
    helios::engine::core::data::SpawnProfileId ProjectileSpawnSpawnProfileId{"projectile_spawn"};

    auto& poolManager = gameWorld.addManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
    auto& spawnManager = gameWorld.addManager<helios::engine::runtime::spawn::SpawnManager>();

    poolManager.addPoolConfig(std::make_unique<helios::engine::runtime::pooling::GameObjectPoolConfig>(
                ProjectilePoolId,
                std::move(projectilePrefab),
                50
            ));

    auto projectileSpawnSpawnProfile = std::make_unique<helios::engine::runtime::spawn::SpawnProfile>(
        ProjectilePoolId,
        std::make_unique<helios::engine::runtime::spawn::behavior::placements::EmitterSpawnPlacer>(),
        std::make_unique<helios::engine::runtime::spawn::behavior::initializers::EmitterInitializer>()
    );

    spawnManager.addSpawnProfile(ProjectileSpawnSpawnProfileId, std::move(projectileSpawnSpawnProfile));

    // --------------------------
    //     GAMELOOP PHASES SETUP
    // --------------------------

    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::SpawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::SpawnCommandDispatcher>()
    );
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::DespawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::DespawnCommandDispatcher>()
    );

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass()
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(*shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::mechanics::combat::systems::ProjectileSpawnSystem>(ProjectileSpawnSpawnProfileId)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)
            .addPass()
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Main)
            .addPass()
            .addSystem<helios::engine::modules::physics::collision::systems::BoundsUpdateSystem>()
            .addSystem<helios::engine::mechanics::bounds::systems::LevelBoundsBehaviorSystem>()
            .addCommitPoint();

    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass()
             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()
             .addSystem<helios::engine::modules::systems::scene::SceneSyncSystem>(scene.get())
             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>();


    float DELTA_TIME = 0.0f;


    // ========================================
    // 6. Activate GameObjects and Initialize
    // ========================================

    leftStickGizmo->setActive(true);
    rightStickGizmo->setActive(true);
    shipDirectionGizmo->setActive(true);

    theGrid->setActive(true);
    std::ignore = gameWorld.addGameObject(std::move(theGrid));
    std::ignore = shipGameObject->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode()->addNode(std::move(cameraSceneNode));

    shipGameObject->setActive(true);
    auto* theShipPtr = gameWorld.addGameObject(std::move(shipGameObject));
    auto* leftStickGizmoNode = leftStickGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo->get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    // Register the spaceship with the tuning widget
    spaceshipWidget->addGameObject("Player 1", theShipPtr);

    // ENGINE INIT
    gameWorld.init();
    gameLoop.init(gameWorld);

    // ========================================
    // 7. Main Game Loop
    // ========================================

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // ----------------------------------------
        // 7.1 Event and Input Processing
        // ----------------------------------------
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        // Check for ESC key to close the application
        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC] - Exiting..." << std::endl;
            win->setShouldClose(true);
        }

        // ----------------------------------------
        // 7.2 Game Logic Update
        // ----------------------------------------
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);

        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot);


        // ----------------------------------------
        // 7.3 Gizmo / Debug Visualization Update
        // ----------------------------------------
        const auto* mcLft = theShipPtr->get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = theShipPtr->get<helios::engine::mechanics::combat::components::Aim2DComponent>();
        if (mcRgt) {
            rightStickGizmoNode->setScale((mcRgt->direction() * mcRgt->frequency()  * 4.0f).toVec3());
        }

        // ----------------------------------------
        // 7.4 Rendering
        // ----------------------------------------
        const auto& snapshot = scene->createSnapshot(*mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        // ----------------------------------------
        // 7.5 ImGui Rendering
        // ----------------------------------------
        imguiOverlay.render();

        // ----------------------------------------
        // 7.6 Frame Synchronization
        // ----------------------------------------
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
