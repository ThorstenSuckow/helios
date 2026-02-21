// ============================================================================
// Includes
// ============================================================================
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <glad/gl.h>
#include <iostream>
#include <numbers>

// ============================================================================
// Module Imports
// ============================================================================
import helios;
import helios.ext;
import helios.examples.scoring;
import helios.examples.scoring.SpaceshipWidget;

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

    using namespace helios::examples::scoring;

    // ========================================
    // 1. Constants
    // ========================================
    constexpr float SCREEN_WIDTH  = 1980;
    constexpr float SCREEN_HEIGHT = 1080;
    constexpr float FOVY          = helios::math::radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    // ========================================
    // 2. Application and Window Setup
    // ========================================
    helios::engine::bootstrap::registerAllComponents();

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Scoring Demo", SCREEN_WIDTH, SCREEN_HEIGHT, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
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
    // 2.1 Title Viewport
    // ----------------------------------------
    auto titleScene = std::make_unique<helios::scene::Scene>(
       std::make_unique<helios::scene::CullNoneStrategy>(),
       helios::engine::core::data::SceneId{"titleScene"}
    );

    auto titleViewport = std::make_shared<helios::rendering::Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, helios::engine::core::data::ViewportId{"titleViewport"}
    );
    sceneToViewportMap.add(titleScene.get(), titleViewport.get());

    titleViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Depth));

    // Orthographic camera for 2D title rendering (origin at bottom-left).
    auto titleCamera = std::make_unique<helios::scene::Camera>();
    auto titleCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(titleCamera));
    auto* titleCameraSceneNode_ptr = titleCameraSceneNode.get();

    titleCameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    titleViewport->setCameraSceneNode(titleCameraSceneNode_ptr);
    titleCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    titleCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    titleCameraSceneNode_ptr->lookAtLocal(
        helios::math::vec3f(0.0f, 0.0f, 0.0f),
        helios::math::vec3f(0.0f, 1.0f, 0.0f)
    );

    // Camera must be attached before the viewport is added to the window.
    win->addViewport(titleViewport);
    std::ignore = titleScene->addNode(std::move(titleCameraSceneNode));

    // ----------------------------------------
    // 2.2 HUD Viewport
    // ----------------------------------------
    auto hudScene = std::make_unique<helios::scene::Scene>(
       std::make_unique<helios::scene::CullNoneStrategy>(),
       helios::engine::core::data::SceneId{"hudScene"}
    );

    auto hudViewport = std::make_shared<helios::rendering::Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, helios::engine::core::data::ViewportId{"hudViewport"}
    );
    sceneToViewportMap.add(hudScene.get(), hudViewport.get());

    hudViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Depth));

    // Orthographic camera for HUD rendering (origin at bottom-left).
    auto hudCamera = std::make_unique<helios::scene::Camera>();
    auto hudCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(hudCamera));
    auto* hudCameraSceneNode_ptr = hudCameraSceneNode.get();

    hudCameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    hudViewport->setCameraSceneNode(hudCameraSceneNode_ptr);
    hudCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    hudCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    hudCameraSceneNode_ptr->lookAtLocal(
        helios::math::vec3f(0.0f, 0.0f, 0.0f),
        helios::math::vec3f(0.0f, 1.0f, 0.0f)
    );

    win->addViewport(hudViewport);
    std::ignore = hudScene->addNode(std::move(hudCameraSceneNode));

    // ----------------------------------------
    // 2.3 Menu Viewport
    // ----------------------------------------
    auto menuScene = std::make_unique<helios::scene::Scene>(
       std::make_unique<helios::scene::CullNoneStrategy>(),
       helios::engine::core::data::SceneId{"menuScene"}
    );

    auto menuViewport = std::make_shared<helios::rendering::Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, helios::engine::core::data::ViewportId{"menuViewport"}
    );
    sceneToViewportMap.add(menuScene.get(), menuViewport.get());

    menuViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Depth));

    // Orthographic camera for menu rendering (origin at bottom-left).
    auto menuCamera = std::make_unique<helios::scene::Camera>();
    auto menuCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(menuCamera));
    auto* menuCameraSceneNode_ptr = menuCameraSceneNode.get();

    menuCameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    menuViewport->setCameraSceneNode(menuCameraSceneNode_ptr);
    menuCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    menuCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    menuCameraSceneNode_ptr->lookAtLocal(
        helios::math::vec3f(0.0f, 0.0f, 0.0f),
        helios::math::vec3f(0.0f, 1.0f, 0.0f)
    );

    win->addViewport(menuViewport);
    std::ignore = menuScene->addNode(std::move(menuCameraSceneNode));

    // ----------------------------------------
    // 2.4 ImGui and Debug Tooling
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
    auto spaceshipWidget = new helios::examples::scoring::SpaceshipWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(gamepadWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);
    imguiOverlay.addWidget(spaceshipWidget);

    // ----------------------------------------
    // 2.5 Logger Configuration
    // ----------------------------------------
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

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

    // ----------------------------------------
    // 3.1 Text Shader
    // ----------------------------------------
    auto glyphUniformLocationMap = std::make_unique<
        helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap
    >();
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, 1);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextTexture, 3);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextColor, 4);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ModelMatrix, 8);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ViewMatrix, 9);

    auto glyphShader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
        "resources/font_shader.vert",
        "resources/font_shader.frag",
        helios::util::io::BasicStringFileReader()
    );
    glyphShader->setUniformLocationMap(std::move(glyphUniformLocationMap));

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
     -(ArenaConfig::GRID_X * ArenaConfig::CELL_LENGTH)/2.0f, -(ArenaConfig::GRID_Y * ArenaConfig::CELL_LENGTH)/2.0f, 0.0f,
            (ArenaConfig::GRID_X * ArenaConfig::CELL_LENGTH)/2.0f, (ArenaConfig::GRID_Y * ArenaConfig::CELL_LENGTH)/2.0f, 0.0f
        },
        helios::core::units::Unit::Meter
    );
    gameWorld.setLevel(std::move(level));

    // ----------------------------------------
    // 5.1 Menu and UI Configuration
    // ----------------------------------------
    configureMenus(gameWorld, app->renderingDevice(), glyphShader, defaultShader, *titleScene, *menuScene, *hudScene);


    // ========================================
    // 6. GameObjects
    // ========================================

    // Grid
    auto theGrid = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
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
        .withTransform([gridScale = ArenaConfig::GRID_SCALE](auto& tb) {
            tb.transform()
              .translate(helios::math::vec3f(0.0f, 0.0f, 0.5f))
              .scale(gridScale);
        })
        .make();

    // Player ship
    auto shipGameObject = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .asPlayerEntity()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::LineLoop)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Triangle>())
               .attachTo(&root);
        })
        .withTransform([&](auto& tb) {
            tb.transform()
              .scale(ArenaConfig::SPACESHIP_SIZE)
              .translate(ArenaConfig::SPACESHIP_SPAWN_POSITION);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(helios::examples::scoring::CollisionId::Player)
              .useBoundingBox()
              .hitPolicy(helios::engine::modules::physics::collision::types::HitPolicy::OneHit)
              .reportCollisions(true)
              .solidCollisionMask(helios::examples::scoring::CollisionId::Enemy)
              .onSolidCollision(
                  helios::examples::scoring::CollisionId::Enemy,
                  helios::engine::modules::physics::collision::types::CollisionBehavior::Despawn
              );

            cb.levelBoundsCollision()
              .onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior::Bounce);
        })
        .withCombat([](auto& ccb) {
            ccb.weapon()
               .fireRate(5.0f);
        })
        .withScoring([](auto& sb) {
            sb.scorePool()
              .poolId(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});
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

    // Debug gizmos (input visualization)
    auto leftStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::White)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        }).make();

    auto rightStickGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Yellow)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    auto shipDirectionGizmo = helios::engine::builder::gameObject::GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(helios::util::Colors::Red)
               .primitiveType(helios::rendering::mesh::PrimitiveType::Lines)
               .shape(std::make_shared<helios::rendering::asset::shape::basic::Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(helios::math::TransformType::Translation);
        })
        .make();

    // Enemy prefabs
    createEnemyPrefabs(gameWorld, shipGameObject, defaultShader);

    // ========================================
    // 7. Manager Registration
    // ========================================

    auto& poolManager      = gameWorld.addManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
    auto& scorePoolManager = gameWorld.addManager<helios::engine::mechanics::scoring::ScorePoolManager>();
    auto& spawnManager     = gameWorld.addManager<helios::engine::runtime::spawn::SpawnManager>();
    auto& gameStateManager = gameWorld.addManager<helios::engine::mechanics::gamestate::GameStateManager>(
        helios::engine::mechanics::gamestate::rules::DefaultGameStateTransitionRules::rules());
    auto& matchStateManager = gameWorld.addManager<helios::engine::mechanics::match::MatchStateManager>(
        helios::engine::mechanics::match::rules::DefaultMatchStateTransitionRules::rules());
    auto& uiActionCommandManager = gameWorld.addManager<helios::engine::modules::ui::UiActionCommandManager>();

    // State listeners and UI action policies
    installMatchStateListeners(matchStateManager);
    installGameStateListeners(gameStateManager);
    applyUiActionCommandPolicies(uiActionCommandManager);

    scorePoolManager.addScorePool(helios::engine::core::data::ScorePoolId{"playerOneScorePool"});

    // Spawn system
    configureSpawns(poolManager, spawnManager);


    // ========================================
    // 8. Command Dispatchers
    // ========================================
    gameLoop.commandBuffer().addDispatcher<helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::ScheduledSpawnPlanCommandDispatcher>()
    ).addDispatcher<helios::engine::runtime::spawn::commands::SpawnCommand>(
        std::make_unique<helios::engine::runtime::spawn::dispatcher::SpawnCommandDispatcher>()
    ).addDispatcher<helios::engine::runtime::spawn::commands::DespawnCommand>(
     std::make_unique<helios::engine::runtime::spawn::dispatcher::DespawnCommandDispatcher>()
    ).addDispatcher<helios::engine::mechanics::scoring::commands::UpdateScoreCommand>(
        std::make_unique<helios::engine::mechanics::scoring::ScoreCommandDispatcher>()
    ).addDispatcher<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
    std::make_unique<helios::engine::state::dispatcher::StateCommandDispatcher<helios::engine::mechanics::gamestate::types::GameState>>()
    ).addDispatcher<helios::engine::state::commands::StateCommand<helios::engine::mechanics::match::types::MatchState>>(
        std::make_unique<helios::engine::state::dispatcher::StateCommandDispatcher<helios::engine::mechanics::match::types::MatchState>>()
    ).addDispatcher<helios::engine::modules::ui::commands::UiActionCommand>(
        std::make_unique<helios::engine::modules::ui::commands::UiActionCommandDispatcher>()
    );

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::core::data;

    // ----------------------------------------
    // 8.1 State-to-ID Mappings
    // ----------------------------------------
    auto stateToViewportMap = helios::engine::state::StateToIdMapPair<
        GameState, MatchState, ViewportId
    >();

    stateToViewportMap.add(GameState::Any, ViewportId{"mainViewport"})
                      .add(GameState::Title, ViewportId{"titleViewport"})
                      .add(GameState::Paused | GameState::Running, ViewportId{"menuViewport"})
                      .add(MatchState::Playing, ViewportId{"hudViewport"});
    stateToViewportMap.freeze();

    auto stateToMenuMap = helios::engine::state::CombinedStateToIdMapPair<
        GameState, MatchState, MenuId
    >();

    stateToMenuMap.add(GameState::Paused, MatchState::Undefined, MenuId{"MainMenu"})
                  .add(GameState::Running, MatchState::GameOver, MenuId{"GameOverMenu"});
    stateToMenuMap.freeze();

    // ========================================
    // 9. GameLoop Phase Configuration
    // ========================================
    //
    //              +----------+
    //              |  FRAME N |
    //              +----------+
    //
    //      PRE    -    MAIN   -    POST
    //    UPDATE      RESOLVE      CLEANUP

    auto Any = GameState::Any;
    auto Running = GameState::Running;
    auto Paused = GameState::Paused;
    auto Title = GameState::Title;
    auto Start = GameState::Start;

    auto RunningOrTitle = GameState::Running |
                          GameState::Title;
    auto RunningOrPaused = GameState::Running |
                          GameState::Paused;

    // ----------------------------------------
    // 9.1 Pre Phase: Input, Spawning, Movement
    // ----------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Pre)
            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::mechanics::gamestate::systems::GameFlowSystem>()
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)

            .addPass<GameState>(Running | Paused)
            .addSystem<helios::engine::mechanics::match::systems::MatchFlowSystem>()
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)

            .addPass<GameState>(GameState::Any)
            .addSystem<helios::engine::mechanics::gamestate::systems::GameStateInputResponseSystem>()
            .addSystem<helios::engine::mechanics::scoring::systems::ScoreObserverSystem>()
            .addSystem<helios::engine::mechanics::scoring::systems::MaxScoreObserverSystem>()
            .addSystem<helios::engine::mechanics::input::systems::TwinStickInputSystem>(shipGameObject)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<helios::engine::mechanics::spawn::systems::GameObjectSpawnSystem>(spawnManager)
            .addSystem<helios::engine::mechanics::combat::systems::ProjectileSpawnSystem>(IdConfig::ProjectileSpawnSpawnProfileId)
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::Structural)

            .addPass<GameState>(Running | Start | Title | Paused)
            .addSystem<helios::engine::modules::ui::widgets::systems::MenuDisplaySystem<GameState, MatchState>>(stateToMenuMap)
            .addSystem<helios::engine::modules::spatial::transform::systems::ScaleSystem>()
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::PassEvents)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<helios::engine::modules::ai::systems::ChaseSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SteeringSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::SpinSystem>()
            .addSystem<helios::engine::modules::physics::motion::systems::Move2DSystem>();

    // ----------------------------------------
    // 9.2 Main Phase: Collision Detection and Response
    // ----------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Main)
            .addPass(GameState::Any)
            .addSystem<helios::engine::ecs::systems::HierarchyPropagationSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::BoundsUpdateSystem>()
            .addCommitPoint()

            .addPass<GameState>(Running | Start | Title)
            .addSystem<helios::engine::mechanics::bounds::systems::LevelBoundsBehaviorSystem>()
            .addSystem<helios::engine::modules::physics::collision::systems::GridCollisionDetectionSystem>(
                levelPtr->bounds(), ArenaConfig::CELL_LENGTH/2.0f
             )
            .addCommitPoint()

            .addPass<GameState>(Running | Start |Title)
            .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateResponseSystem>()
            .addCommitPoint(helios::engine::runtime::gameloop::CommitPoint::PassEvents)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<helios::engine::mechanics::damage::systems::DamageOnCollisionSystem>()
            .addSystem<helios::engine::mechanics::health::systems::HealthUpdateSystem>();

    // ----------------------------------------
    // 9.3 Post Phase: UI, Transform, Rendering, Cleanup
    // ----------------------------------------
    gameLoop.phase(helios::engine::runtime::gameloop::PhaseType::Post)
             .addPass<GameState>(Running | Paused)
             .addSystem<helios::engine::modules::ui::widgets::systems::MenuNavigationSystem>()
             .addSystem<helios::engine::modules::ui::widgets::systems::UiStyleUpdateSystem>()
             .addCommitPoint()

             .addPass<GameState>(GameState::Any)
             .addSystem<helios::engine::modules::ui::binding::systems::Score2UiTextUpdateSystem>()
             .addSystem<helios::engine::modules::ui::binding::systems::MaxScore2UiTextUpdateSystem>()
             .addSystem<helios::engine::mechanics::scoring::systems::CombatScoringSystem>()

             .addSystem<helios::engine::modules::ui::widgets::systems::UiTextBoundsUpdateSystem>()
             .addSystem<helios::engine::modules::ui::transform::systems::UiTransformSystem>()

             .addSystem<helios::engine::modules::spatial::transform::systems::ComposeTransformSystem>()

             .addSystem<
                 helios::engine::modules::rendering::viewport::systems::StateToViewportPolicyUpdateSystem
                    <GameState, MatchState>>(stateToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneSyncSystem>(sceneToViewportMap)
             .addSystem<helios::engine::modules::scene::systems::SceneRenderingSystem>(
                 app->renderingDevice(), sceneToViewportMap)


             .addSystem<helios::engine::modules::spatial::transform::systems::TransformClearSystem>()
             .addSystem<helios::engine::mechanics::lifecycle::systems::DelayedComponentEnablerSystem>()
             .addSystem<helios::engine::modules::physics::collision::systems::CollisionStateClearSystem>()
             .addSystem<helios::engine::mechanics::scoring::systems::ScoreObserverClearSystem>()
             .addSystem<helios::engine::mechanics::scoring::systems::MaxScoreObserverClearSystem>();

    // ========================================
    // 10. Initialization and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    theGrid.setActive(true);
    std::ignore = scene->addNode(std::move(cameraSceneNode));

    auto* leftStickGizmoNode = leftStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo.get<helios::engine::modules::scene::components::SceneNodeComponent>()->sceneNode();

    spaceshipWidget->addGameObject("Player 1", shipGameObject);

    gameWorld.init();
    gameLoop.init(gameWorld);

    bool showImgui = false;
    bool tilde = false;

    gameWorld.session().setStateFrom<GameState>(
        StateTransitionContext<GameState>(GameState::Undefined, GameState::Start, GameStateTransitionId::StartRequested)
    );

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

        // 10.2 Game Logic Update
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::input::InputSnapshot(gamepadState);

        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);

        // 10.3 Debug Gizmo Update
        const auto* mcLft = shipGameObject.get<helios::engine::modules::physics::motion::components::Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = shipGameObject.get<helios::engine::mechanics::combat::components::Aim2DComponent>();
        if (mcRgt) {
            rightStickGizmoNode->setScale((mcRgt->direction() * mcRgt->frequency()  * 4.0f).toVec3());
        }

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
