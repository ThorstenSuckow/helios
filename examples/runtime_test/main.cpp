// ============================================================================
// Includes
// ============================================================================
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <numbers>
#include <random>
#include <vector>
#include "imgui.h"

// ============================================================================
// Module Imports
// ============================================================================
import helios;
import helios.ext;

// ============================================================================
// Using Declarations
// ============================================================================
using namespace helios::ext::glfw::app;
using namespace helios::ext::glfw::window;
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::ext::imgui;
using namespace helios::ext::imgui::widgets;

using namespace helios::rendering;
using namespace helios::rendering::mesh;
using namespace helios::rendering::material;
using namespace helios::rendering::shader;
using namespace helios::rendering::asset::shape::basic;

using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::input::types;

using namespace helios::math;
using namespace helios::scene;
using namespace helios::core::units;
using namespace helios::util;
using namespace helios::util::io;
using namespace helios::util::log;
using namespace helios::util::time;

using namespace helios::engine::bootstrap;
using namespace helios::engine::common::types;
using namespace helios::engine::state;
using namespace helios::engine::state::types;
using namespace helios::engine::tooling;

using namespace helios::engine::ecs::systems;

using namespace helios::engine::builder::gameObject;
using namespace helios::engine::builder::spawnSystem;

using namespace helios::engine::runtime::world;
using namespace helios::engine::runtime::gameloop;
using namespace helios::engine::runtime::pooling;
using namespace helios::engine::runtime::pooling::types;
using namespace helios::engine::runtime::spawn;
using namespace helios::engine::runtime::spawn::types;

using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::mechanics::spawn::systems;

using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::modules::spatial::transform::systems;
using namespace helios::engine::modules::scene::systems;
using namespace helios::engine::modules::scene::components;
using namespace helios::engine::modules::scene::types;
using namespace helios::engine::modules::rendering::viewport::systems;
using namespace helios::engine::modules::physics::motion::systems;
using namespace helios::engine::modules::physics::collision::systems;

using namespace helios::engine::modules::physics::motion::components;
using namespace helios::engine::modules::spatial::transform::components;

using namespace helios::engine::mechanics::bounds::systems;

using namespace helios::engine::runtime::spawn::behavior::placements;
using namespace helios::engine::runtime::spawn::behavior::initializers;

/**
 * @brief ImGui widget to control the number of active pooled GameObjects.
 *
 * Uses a slider to set a target count. Each frame, objects are acquired from
 * or released back to the pool to match the target. Displays current pool
 * statistics alongside the slider.
 */
class RuntimeTestWidget : public ImGuiWidget {

    GameObjectPoolManager* poolManager_ = nullptr;
    GameWorld* gameWorld_ = nullptr;
    GameObjectPoolId poolId_;
    int targetCount_ = 0;
    int maxPoolSize_ = 0;

    // the answer to everything
    uint32_t seed = 42;
    helios::util::Random rGen_{seed};

    /// Tracks currently active (acquired) EntityHandles for ordered release.
    std::vector<helios::engine::ecs::EntityHandle> activeHandles_;

public:
    RuntimeTestWidget(
        GameObjectPoolManager* poolManager,
        GameWorld* gameWorld,
        GameObjectPoolId poolId,
        int maxPoolSize
    ) : poolManager_(poolManager),
        gameWorld_(gameWorld),
        poolId_(poolId),
        maxPoolSize_(maxPoolSize) {
        activeHandles_.reserve(maxPoolSize);
    }

    /**
     * @brief Synchronizes the active object count with the slider target.
     *
     * Call once per frame **before** the game loop update so that
     * acquire/release operations are reflected in the current frame.
     */
    void sync() {
        if (!poolManager_ || !gameWorld_) return;

        const auto& bounds = gameWorld_->level()->bounds();

        auto randomSpawnPlacer = RandomSpawnPlacer();
        auto moveInitializer = MoveInitializer(DirectionType::Random);

        // Acquire objects until we reach the target
        int position = 0;
        while (static_cast<int>(activeHandles_.size()) < targetCount_) {
            auto go = poolManager_->acquire(poolId_);
            if (!go) break; // pool exhausted

            auto spawnPlanCursor = SpawnPlanCursor{static_cast<uint32_t>(targetCount_), static_cast<uint32_t>(position++)};

            // Random placement within level bounds
            auto* tsc = go->get<TranslationStateComponent>();
            tsc->setTranslation(randomSpawnPlacer.getPosition(
                go->entityHandle(),
                go->get<helios::engine::modules::physics::collision::components::AabbColliderComponent>()->bounds(),
                bounds, spawnPlanCursor, {}
            ));

            moveInitializer.initialize(*go, spawnPlanCursor, {});

            go->setActive(true);
            activeHandles_.push_back(go->entityHandle());
        }

        // Release objects until we reach the target
        while (static_cast<int>(activeHandles_.size()) > targetCount_) {
            auto handle = activeHandles_.back();
            activeHandles_.pop_back();
            poolManager_->release(poolId_, handle);
        }
    }

    void draw() override {
        ImGui::SetNextWindowSize(ImVec2(400, 180), ImGuiCond_FirstUseEver);

        if (!ImGui::Begin("Runtime Stress Test", nullptr, ImGuiWindowFlags_NoCollapse)) {
            ImGui::End();
            return;
        }

        ImGui::SeparatorText("Object Pool Control");

        ImGui::SliderInt("Target Objects", &targetCount_, 0, maxPoolSize_);

        if (poolManager_) {
            auto snapshot = poolManager_->poolSnapshot(poolId_);
            ImGui::Text("Active:   %zu", snapshot.activeCount);
            ImGui::Text("Inactive: %zu", snapshot.inactiveCount);
        }

        ImGui::Spacing();

        // Quick preset buttons
        if (ImGui::Button("0")) { targetCount_ = 0; }
        ImGui::SameLine();
        if (ImGui::Button("100")) { targetCount_ = std::min(100, maxPoolSize_); }
        ImGui::SameLine();
        if (ImGui::Button("500")) { targetCount_ = std::min(500, maxPoolSize_); }
        ImGui::SameLine();
        if (ImGui::Button("1000")) { targetCount_ = std::min(1000, maxPoolSize_); }
        ImGui::SameLine();
        if (ImGui::Button("2000")) { targetCount_ = std::min(2000, maxPoolSize_); }
        ImGui::SameLine();
        if (ImGui::Button("5000")) { targetCount_ = std::min(5000, maxPoolSize_); }
        ImGui::SameLine();
        if (ImGui::Button("Max")) { targetCount_ = maxPoolSize_; }

        ImGui::End();
    }
};

// ============================================================================
// Entry Point
// ============================================================================
int main() {

    // ========================================
    // 1. Constants
    // ========================================
    constexpr float CELL_SIZE          = 5.0f;
    constexpr float OBJECT_SIZE        = 3.0f;
    constexpr float GRID_X             = 29.0f;
    constexpr float GRID_Y             = 19.0f;
    constexpr float FOVY               = radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;
    constexpr int   POOL_SIZE          = 10'000;

    constexpr PrefabId   StressObjectPrefabId{"stress_object"};
    constexpr GameObjectPoolId StressPoolId{"stress_pool"};
    constexpr SpawnProfileId   StressSpawnProfileId{"stress_spawn"};

    // ========================================
    // 2. Application and Window Setup
    // ========================================
    registerAllComponents();

    auto [gameWorldPtr, gameLoopPtr] = makeGameWorld(POOL_SIZE + 100);
    auto& gameWorld = *gameWorldPtr;
    auto& gameLoop  = *gameLoopPtr;

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Runtime Stress Test", 1980, 1080, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto sceneToViewportMap = SceneToViewportMap();
    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f,
        ViewportId{"mainViewport"});

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.02f, 0.02f, 0.08f, 1.0f));
    win->addViewport(mainViewport);

    InputManager& inputManager = app->inputManager();

    const auto basicStringFileReader = BasicStringFileReader();

    // ----------------------------------------
    // 2.1 ImGui and Debug Tooling
    // ----------------------------------------
    auto imguiBackend = ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics   = FpsMetrics();
    auto stopwatch    = std::make_unique<Stopwatch>();
    auto framePacer   = FramePacer(std::move(stopwatch));
    framePacer.setTargetFps(0.0f);
    FrameStats frameStats{};

    auto menu         = new MainMenuWidget();
    auto fpsWidget    = new FpsWidget(&fpsMetrics, &framePacer);
    auto logWidget    = new LogWidget();
    auto cameraWidget = new CameraWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);

    // RuntimeTestWidget is created after pool registration (see below)

    // ----------------------------------------
    // 2.2 Logger Configuration
    // ----------------------------------------
    LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<ImGuiLogSink>(logWidget);
    LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 3. Shader Creation
    // ========================================
    auto defaultShader =
            std::make_shared<OpenGLShader>(
                "./resources/cube.vert",
                "./resources/cube.frag", basicStringFileReader);

    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    defaultShader->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 4. Scene Graph and Camera Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<Scene>(
        std::move(frustumCullingStrategy), SceneId{"mainScene"});
    sceneToViewportMap.add(scene.get(), mainViewport.get());

    auto mainViewportCam = std::make_unique<Camera>();
    auto cameraSceneNode = std::make_unique<CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();

    cameraSceneNode_ptr->setInheritance(TransformType::Translation);
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(vec3f(0.0f, 0.0f, -100.0f));
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
    auto level = std::make_unique<Level>(&(scene.get()->root()));
    auto* levelPtr = level.get();
    level->setBounds(
        aabb{
            -(GRID_X * CELL_SIZE) / 2.0f, -(GRID_Y * CELL_SIZE) / 2.0f, 0.0f,
             (GRID_X * CELL_SIZE) / 2.0f,  (GRID_Y * CELL_SIZE) / 2.0f, 0.0f
        },
        Unit::Meter
    );
    gameWorld.setLevel(std::move(level));

    // ========================================
    // 6. GameObjects
    // ========================================

    // Grid (background visualization)
    auto theGrid = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Turquoise.withW(0.15f))
               .primitiveType(PrimitiveType::Lines)
               .shape(std::make_shared<Grid>(29, 19))
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .translate(vec3f(0.0f, 0.0f, 0.5f))
              .scale(vec3f(GRID_X * CELL_SIZE, GRID_Y * CELL_SIZE, 0.0f));
        })
        .make();

    // Stress-test object prefab
    // here's our infamous purple enemy, this time sporting the color pink
    auto stressObjectPrefab = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withPrefabId(StressObjectPrefabId)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Pink)
               .primitiveType(PrimitiveType::LineLoop)
               .shape(std::make_shared<Rectangle>())
               .attachTo(&root);
        })
        .withTransform([](auto& tb) {
            tb.transform()
              .scale(vec3f(OBJECT_SIZE / 2.0f, OBJECT_SIZE / 2.0f, 0.0f), Unit::Meter);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Reflect);
        })
        .withMotion([](auto& mcb) {
            mcb.move2D()
               .speed(5.0f)
               .instantAcceleration(true);
        })
        .withEffects([](auto& eb) {
            eb.gfx()
              .spin(180.0f, Z_AXISf);
        })
        .withSpawn([](auto& sb) {
            sb.spawn()
              .useSpawnProfile();
        })
        .make();

    // ========================================
    // 7. Manager Registration
    // ========================================
    auto& poolManager  = gameWorld.registerManager<GameObjectPoolManager>();
    auto& spawnManager = gameWorld.registerManager<SpawnManager>();

    // Pool configuration (no automatic spawning — the widget drives acquire/release)
    SpawnSystemFactory::configure(poolManager, spawnManager)
        .pool(StressPoolId, StressObjectPrefabId, POOL_SIZE)
            .profile(StressSpawnProfileId)
                .randomPlacement()
                .randomDirectionInitializer()
                .done()
            .commitProfilesOnly();

    // ----------------------------------------
    // 7.1 RuntimeTestWidget (needs poolManager)
    // ----------------------------------------
    auto runtimeTestWidget = new RuntimeTestWidget(&poolManager, &gameWorld, StressPoolId, POOL_SIZE);
    imguiOverlay.addWidget(runtimeTestWidget);

    // ----------------------------------------
    // 8. State-to-Viewport Mapping
    // ----------------------------------------
    auto stateToViewportMap = StateToIdMapPair<
        GameState, MatchState, ViewportId
    >();

    stateToViewportMap.add(GameState::Any, ViewportId{"mainViewport"});
    stateToViewportMap.freeze();

    // ========================================
    // 9. GameLoop Phase Configuration
    // ========================================
    gameLoop.phase(PhaseType::Pre)
            .addPass<GameState>(GameState::Any)
            .addSystem<GameObjectSpawnSystem>(spawnManager)
            .addCommitPoint(CommitPoint::Structural)
            .addPass<GameState>(GameState::Any)
            .addSystem<ScaleSystem>()
            .addSystem<SpinSystem>()
            .addSystem<Move2DSystem>();

    gameLoop.phase(PhaseType::Main)
            .addPass<GameState>(GameState::Any)
            .addSystem<HierarchyPropagationSystem>()
            .addSystem<BoundsUpdateSystem>()
            .addSystem<LevelBoundsBehaviorSystem>()
            .addCommitPoint();

    gameLoop.phase(PhaseType::Post)
             .addPass<GameState>(GameState::Any)
             .addSystem<ComposeTransformSystem>()
             .addSystem<
                 StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
             .addSystem<SceneSyncSystem>(sceneToViewportMap)
             .addSystem<SceneRenderingSystem>(
                 app->renderingDevice(), sceneToViewportMap)
             .addSystem<TransformClearSystem>();

    // ========================================
    // 10. Initialization and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    theGrid.setActive(true);
    std::ignore = scene->addNode(std::move(cameraSceneNode));

    gameWorld.init();
    gameLoop.init(gameWorld);

    gameWorld.session().setStateFrom<GameState>(
        StateTransitionContext<GameState>(GameState::Undefined, GameState::Booted, GameStateTransitionId::BootRequest)
    );

    bool showImgui = true;
    bool tilde = false;

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // 10.1 Event and Input Processing
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

        // 10.2 Pool sync — acquire/release objects to match slider target
        runtimeTestWidget->sync();

        // 10.3 Game Logic Update
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = InputSnapshot(gamepadState);

        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);

        // 10.4 ImGui Overlay
        if (showImgui) {
            imguiOverlay.render();
        }

        // 10.5 Frame Synchronization
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}





