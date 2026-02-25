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
// Namespaces
// ============================================================================
#include "Namespaces.h"

// ============================================================================
// Entry Point
// ============================================================================

int main() {


    // ========================================
    // 1. Constants
    // ========================================
    constexpr float SCREEN_WIDTH  = 1980;
    constexpr float SCREEN_HEIGHT = 1080;
    constexpr float FOVY          = radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    // ========================================
    // 2. Application and Window Setup
    // ========================================
    registerAllComponents();

    auto [gameWorldPtr, gameLoopPtr] = makeGameWorld();
    auto& gameWorld = *gameWorldPtr;
    auto& gameLoop = *gameLoopPtr;

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Scoring Demo", SCREEN_WIDTH, SCREEN_HEIGHT, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto sceneToViewportMap = SceneToViewportMap();
    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f,
        ViewportId{"mainViewport"});

    mainViewport->setClearFlags(std::to_underlying(ClearFlags::Color))
                  .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    InputManager& inputManager = app->inputManager();
    unsigned int mask = inputManager.registerGamepads(Gamepad::ONE);

    const auto basicStringFileReader = BasicStringFileReader();

    // ----------------------------------------
    // 2.1 Title Viewport
    // ----------------------------------------
    auto titleScene = std::make_unique<Scene>(
       std::make_unique<CullNoneStrategy>(),
       SceneId{"titleScene"}
    );

    auto titleViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, ViewportId{"titleViewport"}
    );
    sceneToViewportMap.add(titleScene.get(), titleViewport.get());

    titleViewport->setClearFlags(std::to_underlying(ClearFlags::Depth));

    // Orthographic camera for 2D title rendering (origin at bottom-left).
    auto titleCamera = std::make_unique<Camera>();
    auto titleCameraSceneNode = std::make_unique<CameraSceneNode>(std::move(titleCamera));
    auto* titleCameraSceneNode_ptr = titleCameraSceneNode.get();

    titleCameraSceneNode_ptr->setInheritance(TransformType::Translation);
    titleViewport->setCameraSceneNode(titleCameraSceneNode_ptr);
    titleCameraSceneNode_ptr->setTranslation(vec3f(0.0f, 0.0f, -100.0f));
    titleCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    titleCameraSceneNode_ptr->lookAtLocal(
        vec3f(0.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f)
    );

    // Camera must be attached before the viewport is added to the window.
    win->addViewport(titleViewport);
    std::ignore = titleScene->addNode(std::move(titleCameraSceneNode));

    // ----------------------------------------
    // 2.2 HUD Viewport
    // ----------------------------------------
    auto hudScene = std::make_unique<Scene>(
       std::make_unique<CullNoneStrategy>(),
       SceneId{"hudScene"}
    );

    auto hudViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, ViewportId{"hudViewport"}
    );
    sceneToViewportMap.add(hudScene.get(), hudViewport.get());

    hudViewport->setClearFlags(std::to_underlying(ClearFlags::Depth));

    // Orthographic camera for HUD rendering (origin at bottom-left).
    auto hudCamera = std::make_unique<Camera>();
    auto hudCameraSceneNode = std::make_unique<CameraSceneNode>(std::move(hudCamera));
    auto* hudCameraSceneNode_ptr = hudCameraSceneNode.get();

    hudCameraSceneNode_ptr->setInheritance(TransformType::Translation);
    hudViewport->setCameraSceneNode(hudCameraSceneNode_ptr);
    hudCameraSceneNode_ptr->setTranslation(vec3f(0.0f, 0.0f, -100.0f));
    hudCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    hudCameraSceneNode_ptr->lookAtLocal(
        vec3f(0.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f)
    );

    win->addViewport(hudViewport);
    std::ignore = hudScene->addNode(std::move(hudCameraSceneNode));

    // ----------------------------------------
    // 2.3 Menu Viewport
    // ----------------------------------------
    auto menuScene = std::make_unique<Scene>(
       std::make_unique<CullNoneStrategy>(),
       SceneId{"menuScene"}
    );

    auto menuViewport = std::make_shared<Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f, ViewportId{"menuViewport"}
    );
    sceneToViewportMap.add(menuScene.get(), menuViewport.get());

    menuViewport->setClearFlags(std::to_underlying(ClearFlags::Depth));

    // Orthographic camera for menu rendering (origin at bottom-left).
    auto menuCamera = std::make_unique<Camera>();
    auto menuCameraSceneNode = std::make_unique<CameraSceneNode>(std::move(menuCamera));
    auto* menuCameraSceneNode_ptr = menuCameraSceneNode.get();

    menuCameraSceneNode_ptr->setInheritance(TransformType::Translation);
    menuViewport->setCameraSceneNode(menuCameraSceneNode_ptr);
    menuCameraSceneNode_ptr->setTranslation(vec3f(0.0f, 0.0f, -100.0f));
    menuCameraSceneNode_ptr->camera().setOrtho(
        0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    menuCameraSceneNode_ptr->lookAtLocal(
        vec3f(0.0f, 0.0f, 0.0f),
        vec3f(0.0f, 1.0f, 0.0f)
    );

    win->addViewport(menuViewport);
    std::ignore = menuScene->addNode(std::move(menuCameraSceneNode));

    // ----------------------------------------
    // 2.4 ImGui and Debug Tooling
    // ----------------------------------------
    auto imguiBackend = ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics = FpsMetrics();
    auto stopwatch = std::make_unique<Stopwatch>();
    auto framePacer = FramePacer(std::move(stopwatch));
    framePacer.setTargetFps(0.0f);
    FrameStats frameStats{};
    auto menu = new MainMenuWidget();
    auto fpsWidget = new FpsWidget(&fpsMetrics, &framePacer);
    auto gamepadWidget = new GamepadWidget(&inputManager);
    auto logWidget = new LogWidget();
    auto cameraWidget = new CameraWidget();
    auto spaceshipWidget = new SpaceshipWidget();
    imguiOverlay.addWidget(menu);
    imguiOverlay.addWidget(fpsWidget);
    imguiOverlay.addWidget(gamepadWidget);
    imguiOverlay.addWidget(logWidget);
    imguiOverlay.addWidget(cameraWidget);
    imguiOverlay.addWidget(spaceshipWidget);

    // ----------------------------------------
    // 2.5 Logger Configuration
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

    // ----------------------------------------
    // 3.1 Text Shader
    // ----------------------------------------
    auto glyphUniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    glyphUniformLocationMap->set(UniformSemantics::ProjectionMatrix, 1);
    glyphUniformLocationMap->set(UniformSemantics::TextTexture, 3);
    glyphUniformLocationMap->set(UniformSemantics::TextColor, 4);
    glyphUniformLocationMap->set(UniformSemantics::ModelMatrix, 8);
    glyphUniformLocationMap->set(UniformSemantics::ViewMatrix, 9);

    auto glyphShader = std::make_shared<OpenGLShader>(
        "resources/font_shader.vert",
        "resources/font_shader.frag",
        BasicStringFileReader()
    );
    glyphShader->setUniformLocationMap(std::move(glyphUniformLocationMap));

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
     -(ArenaConfig::GRID_X * ArenaConfig::CELL_LENGTH)/2.0f, -(ArenaConfig::GRID_Y * ArenaConfig::CELL_LENGTH)/2.0f, 0.0f,
            (ArenaConfig::GRID_X * ArenaConfig::CELL_LENGTH)/2.0f, (ArenaConfig::GRID_Y * ArenaConfig::CELL_LENGTH)/2.0f, 0.0f
        },
        Unit::Meter
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
    auto theGrid = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Turquoise.withW(0.2f))
               .primitiveType(PrimitiveType::Lines)
               .shape(std::make_shared<Grid>(29, 19))
               .attachTo(&root);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .useBoundingBox();
        })
        .withTransform([gridScale = ArenaConfig::GRID_SCALE](auto& tb) {
            tb.transform()
              .translate(vec3f(0.0f, 0.0f, 0.5f))
              .scale(gridScale);
        })
        .make();

    // Player ship
    auto shipGameObject = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .asPlayerEntity()
        .withRendering([&defaultShader, &root = *levelPtr->rootNode()](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Yellow)
               .primitiveType(PrimitiveType::LineLoop)
               .shape(std::make_shared<Triangle>())
               .attachTo(&root);
        })
        .withTransform([&](auto& tb) {
            tb.transform()
              .scale(ArenaConfig::SPACESHIP_SIZE)
              .translate(ArenaConfig::SPACESHIP_SPAWN_POSITION);
        })
        .withCollision([](auto& cb) {
            cb.collision()
              .layerId(CollisionId::Player)
              .useBoundingBox()
              .hitPolicy(HitPolicy::OneHit)
              .reportCollisions(true)
              .solidCollisionMask(CollisionId::Enemy)
              /*.onSolidCollision(
                  CollisionId::Enemy,
                  CollisionBehavior::Despawn
              )*/;

            cb.levelBoundsCollision()
              .onCollision(CollisionBehavior::Bounce);
        })
        .withCombat([](auto& ccb) {
            ccb.weapon()
               .fireRate(5.0f);
        })
        .withScoring([](auto& sb) {
            sb.scorePool()
              .poolId(ScorePoolId{"playerOneScorePool"});
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
    auto leftStickGizmo = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::White)
               .primitiveType(PrimitiveType::Lines)
               .shape(std::make_shared<Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(TransformType::Translation);
        }).make();

    auto rightStickGizmo = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Yellow)
               .primitiveType(PrimitiveType::Lines)
               .shape(std::make_shared<Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(TransformType::Translation);
        })
        .make();

    auto shipDirectionGizmo = GameObjectFactory::instance()
        .gameObject(gameWorld)
        .withRendering([&defaultShader, shipGameObject](auto& rnb) {
            rnb.meshRenderable()
               .shader(defaultShader)
               .color(Colors::Red)
               .primitiveType(PrimitiveType::Lines)
               .shape(std::make_shared<Line>())
               .build();

            rnb.sceneNode()
               .parent(shipGameObject)
               .inherit(TransformType::Translation);
        })
        .make();

    // Enemy prefabs
    createEnemyPrefabs(gameWorld, shipGameObject, defaultShader);

    // ========================================
    // 7. Manager Registration
    // ========================================

    auto& poolManager      = gameWorld.registerManager<GameObjectPoolManager>();
    auto& scorePoolManager = gameWorld.registerManager<ScorePoolManager>();
    auto& timerManager = gameWorld.registerManager<TimerManager>();
    auto& spawnManager     = gameWorld.registerManager<SpawnManager>();
    auto& uiActionCommandManager = gameWorld.registerManager<UiActionCommandManager>();

    auto& gameStateManager = gameWorld.manager<GameStateManager>();
    auto& matchStateManager = gameWorld.manager<MatchStateManager>();

    // State listeners and UI action policies
    installMatchStateListeners(matchStateManager);
    installGameStateListeners(gameStateManager);
    installDemoTimeListeners(gameStateManager, matchStateManager);
    applyUiActionCommandPolicies(uiActionCommandManager);

    scorePoolManager.addScorePool(ScorePoolId{"playerOneScorePool"});

    // Spawn system
    configureSpawns(poolManager, spawnManager);

    configureTimer(timerManager);




    // ----------------------------------------
    // 8.1 State-to-ID Mappings
    // ----------------------------------------
    auto stateToViewportMap = StateToIdMapPair<
        GameState, MatchState, ViewportId
    >();

    stateToViewportMap.add(GameState::Any, ViewportId{"mainViewport"})
                      .add(GameState::Title, ViewportId{"titleViewport"})
                      .add(GameState::Paused | GameState::Running, ViewportId{"menuViewport"})
                      .add(MatchState::Playing, ViewportId{"hudViewport"});
    stateToViewportMap.freeze();

    auto stateToMenuMap = CombinedStateToIdMapPair<
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
    gameLoop.phase(PhaseType::Pre)
            .addPass<GameState>(GameState::Any)
            .addSystem<GameFlowSystem>()
            .addCommitPoint(CommitPoint::Structural)

            .addPass<GameState>(Running | Paused)
            .addSystem<MatchFlowSystem>()
            .addCommitPoint(CommitPoint::Structural)

            .addPass<GameState>(GameState::Any)
            .addSystem<GameStateInputResponseSystem>()
            .addSystem<ScoreObserverSystem>(scorePoolManager)
            .addSystem<MaxScoreObserverSystem>(scorePoolManager)
            .addSystem<TwinStickInputSystem>(shipGameObject)
            .addCommitPoint(CommitPoint::Structural)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<GameObjectSpawnSystem>(spawnManager)
            .addSystem<ProjectileSpawnSystem>(IdConfig::ProjectileSpawnSpawnProfileId)
            .addCommitPoint(CommitPoint::Structural)

            .addPass<GameState>(Running | Start | Title | Paused)
            .addSystem<MenuDisplaySystem<GameState, MatchState>>(stateToMenuMap)
            .addSystem<ScaleSystem>()
            .addCommitPoint(CommitPoint::PassEvents)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<ChaseSystem>()
            .addSystem<SteeringSystem>()
            .addSystem<SpinSystem>()
            .addSystem<Move2DSystem>();

    // ----------------------------------------
    // 9.2 Main Phase: Collision Detection and Response
    // ----------------------------------------
    gameLoop.phase(PhaseType::Main)
            .addPass(GameState::Any)
            .addSystem<HierarchyPropagationSystem>()
            .addSystem<BoundsUpdateSystem>()
            .addCommitPoint()

            .addPass<GameState>(Running | Start | Title)
            .addSystem<LevelBoundsBehaviorSystem>()
            .addSystem<GridCollisionDetectionSystem>(
                levelPtr->bounds(), ArenaConfig::CELL_LENGTH/2.0f
             )
            .addCommitPoint()

            .addPass<GameState>(Running | Start |Title)
            .addSystem<CollisionStateResponseSystem>()
            .addCommitPoint(CommitPoint::PassEvents)

            .addPass<GameState>(Running | Start | Title)
            .addSystem<DamageOnCollisionSystem>()
            .addSystem<HealthUpdateSystem>();

    // ----------------------------------------
    // 9.3 Post Phase: UI, Transform, Rendering, Cleanup
    // ----------------------------------------
    gameLoop.phase(PhaseType::Post)
             .addPass<GameState>(Running | Paused)
             .addSystem<GameTimerUpdateSystem>(timerManager)
             .addSystem<MenuNavigationSystem>()
             .addSystem<UiStyleUpdateSystem>()
             .addSystem<GameTimer2UiTextUpdateSystem>(timerManager)
             .addCommitPoint()

             .addPass<GameState>(GameState::Any)
             .addSystem<Score2UiTextUpdateSystem>()
             .addSystem<MaxScore2UiTextUpdateSystem>()
             .addSystem<CombatScoringSystem>()

             .addSystem<UiTextBoundsUpdateSystem>()
             .addSystem<UiTransformSystem>()

             .addSystem<ComposeTransformSystem>()

             .addSystem<StateToViewportPolicyUpdateSystem<GameState, MatchState>>(stateToViewportMap)
             .addSystem<SceneSyncSystem>(sceneToViewportMap)
             .addSystem<SceneRenderingSystem>(
                 app->renderingDevice(), sceneToViewportMap)


             .addSystem<TransformClearSystem>()
             .addSystem<DelayedComponentEnablerSystem>()
             .addSystem<CollisionStateClearSystem>()
             .addSystem<ScoreObserverClearSystem>()
             .addSystem<MaxScoreObserverClearSystem>();

    // ========================================
    // 10. Initialization and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    theGrid.setActive(true);
    std::ignore = scene->addNode(std::move(cameraSceneNode));

    auto* leftStickGizmoNode = leftStickGizmo.get<SceneNodeComponent>()->sceneNode();
    auto* rightStickGizmoNode = rightStickGizmo.get<SceneNodeComponent>()->sceneNode();
    auto* shipDirectionGizmoNode = shipDirectionGizmo.get<SceneNodeComponent>()->sceneNode();

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
        const auto inputSnapshot = InputSnapshot(gamepadState);

        const auto viewportSnapshots = win->viewportSnapshots();
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, viewportSnapshots);

        // 10.3 Debug Gizmo Update
        const auto* mcLft = shipGameObject.get<Move2DComponent>();
        if (mcLft) {
            leftStickGizmoNode->setScale((mcLft->direction() * mcLft->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mcLft->velocity().normalize() * mcLft->speedRatio() * 4.0f);
        }
        const auto* mcRgt = shipGameObject.get<Aim2DComponent>();
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
