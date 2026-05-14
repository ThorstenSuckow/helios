import helios;
import helios.ext;

import helios.examples.ecs_rendering.GameLoop;

#include "Namespaces.h"



int main() {

    // ========================================
    // Constants
    // ========================================
    constexpr unsigned int SCREEN_WIDTH  = 1280;
    constexpr unsigned int SCREEN_HEIGHT = 720;

    constexpr float FOVY               = radians(90.0f);
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    constexpr auto SHADER_POOL_CAPACITY   = 10;
    constexpr auto MATERIAL_POOL_CAPACITY = 10;
    constexpr auto MESH_POOL_CAPACITY     = 10;
    constexpr auto FRAMEBUFFER_POOL_CAPACITY = 10;
    constexpr auto VIEWPORT_POOL_CAPACITY    = 10;


    // ==========================================================
    // Infrastructure init / GameWorld / GameLoop / InputManager
    // ==========================================================

    // inputmanager
    auto deadzoneStrategy = RadialDeadzoneStrategy();
    /*const auto inputManager = std::make_unique<InputManager>(
        std::make_unique<helios::ext::glfw::input::GLFWInputAdapter>(std::move(deadzoneStrategy))
    );*/

    // gameworld

    auto [gameWorldPtr, gameLoopPtr] = bootstrapGameWorld();
    auto& gameWorld = *gameWorldPtr;
    auto& gameLoop = *gameLoopPtr;


    // Renderbackend
    auto renderBackend = OpenGLBackend(
        gameWorld.renderResourceWorld(),
        gameWorld.renderTargetWorld()
    );

    // register additional managers
    gameWorld.registerManager<helios::rendering::RenderManager<OpenGLBackend, GameObjectHandle>>(renderBackend);

    gameWorld.registerManager<GLFWPlatformManager<WindowHandle,  /*InputHandle, */ StateCommandBuffer, PlatformCommandBuffer>>(
        gameWorld.platformWorld(), gameWorld.resourceRegistry().commandBufferRegistry()
    );

    gameWorld.registerManager<OpenGLShaderCompileManager<ShaderHandle>>(gameWorld.renderResourceWorld());


    // ========================================
    // Window Setup
    // ========================================
    auto MainWindow = gameWorld.add<WindowHandle>(WindowId("MainWindow"));
    MainWindow.add<WindowCreateRequestComponent<WindowHandle>>(WindowConfig{
        "helios - ECS Rendering Demo",
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        ASPECT_RATIO_NUMER,
        ASPECT_RATIO_DENOM
    });

    
    // ========================================
    // Scene and Viewport Setup
    // ========================================

    auto WindowFramebuffer = gameWorld.add<FramebufferHandle>(FramebufferId{"WindowFramebuffer"});
    WindowFramebuffer.add<OpenGLFramebufferIdComponent<FramebufferHandle>>(0);
    WindowFramebuffer.add<Size2DComponent<FramebufferHandle>>();
    WindowFramebuffer.add<ClearComponent<FramebufferHandle>>(ClearFlags::Color);
    WindowFramebuffer.add<ColorComponent<FramebufferHandle>>(0.5f);
    auto MainViewport = gameWorld.add<ViewportHandle>(ViewportId{"MainViewport"});

    auto MainScene = gameWorld.add<SceneHandle>(SceneId("MainScene"));

    MainWindow.add<FramebufferBindingComponent<WindowHandle>>(WindowFramebuffer);

    // Framebuffer : Viewport (1:N)
    MainViewport.add<FramebufferBindingComponent<ViewportHandle>>(WindowFramebuffer);
    MainViewport.add<BoundsComponent<ViewportHandle>>(helios::math::vec4f{.5f, .5f, .5f, .5f});

    // Viewport : Scene (N:1)
    MainViewport.add<SceneBindingComponent<ViewportHandle>>(MainScene);

    auto player = gameWorld.add<GameObjectHandle>();
    player.add<SceneMemberComponent<GameObjectHandle>>(MainScene);

    auto MainCamera = gameWorld.add<GameObjectHandle>();
    MainCamera.add<PerspectiveCameraComponent<GameObjectHandle>>();
    MainCamera.add<LookAtComponent<GameObjectHandle>>();
    MainCamera.add<ProjectionMatrixComponent<GameObjectHandle>>();
    MainCamera.add<ViewMatrixComponent<GameObjectHandle>>();
    MainCamera.add<DirectionComponent<GameObjectHandle>>();
    MainCamera.add<SceneMemberComponent<GameObjectHandle>>(MainScene);
    // later on: rebuildHandleMultiMapFromSceneMembership(). SSoT w/ components, but systems get the
    // multimaps for faster access / querying?
    // or the view gets extended internally that it can fall back to a multimap, e.g. filter<> instead of view<>
    // or some other adequate semantic name

    MainViewport.add<CameraBindingComponent<ViewportHandle>>(MainCamera);


    // ========================================
    // Rendering Management setup
    // ========================================

    // shader, mesh, material for cube
    auto CubeShader = gameWorld.add<ShaderHandle>(ShaderId("CubeShader"));
    CubeShader.add<ShaderSourceComponent<ShaderHandle>>("./resources/cube.vert", "./resources/cube.frag");
    auto CubeMesh = gameWorld.add<MeshHandle>(MeshId("CubeMesh"));
    auto CubeMaterial = gameWorld.add<MaterialHandle>(MaterialId("CubeMaterial"));


    // ========================================
    // Entity Setup
    // ========================================
    // cube
    auto cube = gameWorld.add<GameObjectHandle>();
    cube.add<SceneMemberComponent<GameObjectHandle>>(MainScene);
    cube.add<LocalPositionStateComponent<GameObjectHandle>>();
    cube.add<LocalToWorldBoundsComponent<GameObjectHandle>>();
    cube.add<LocalToWorldMatrixComponent<GameObjectHandle>>();
    cube.add<RenderPrototypeComponent<GameObjectHandle>>(CubeShader, CubeMaterial, CubeMesh);


    // ==============================================
    // Map Scenes to Viewports, Cameras with Viewports.
    // ==============================================
    //auto mainViewportEntity = gameWorld.addGameObject();
    //mainViewportEntity.add<ViewportComponent>(MainViewportHandle, MainSceneHandle, camera.entityHandle());
    // entities are described through the sum of their parts.
    // if we have a ViewportComponent and a UniformValueMapComponent, those values are treated per frame.



    // ========================================
    // Initialization of GameWorld and Game Loop
    // ========================================
    float DELTA_TIME = 0.0f;

    auto framePacer = FramePacer();
    framePacer.setTargetFps(0.0f);
    FrameStats frameStats{};

    // ----------------------------------------
    // GameLoop Config
    // ----------------------------------------
    gameLoop.phase(PhaseType::Pre)
                .addPass<GameState>(GameState::Any)
                .addSystem<GameFlowSystem<StateCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Booting)
                .addSystem<PlatformInitSystem<PlatformCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Booted | GameState::Live)
                .addSystem<PollEventsSystem<PlatformCommandBuffer>>()
                .addSystem<WindowCreateSystem<WindowHandle, PlatformCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Warmup)
                .addSystem<ShaderCompileSystem<ShaderHandle, RenderCommandBuffer>>()
                .addSystem<WarmupDoneSystem<ShaderHandle, StateCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Live)
                .addSystem<ScaleSystem<GameObjectHandle>>();

            gameLoop.phase(PhaseType::Main)
                .addPass(GameState::Live)
                //.addSystem<PerspectiveProjectionUpdateSystem<GameObjectHandle>>()
                //.addSystem<CameraLookAtSystem<GameObjectHandle>>()
                //.addSystem<ViewMatrixUpdateSystem<GameObjectHandle>>()
                ;

            gameLoop.phase(PhaseType::Post)
                 .addPass(GameState::Live)

                 //.addSystem<LocalComposeTransformSystem>()
                 //.addSystem<WorldTransformSystem>()

                // this will produce render commands after scenes have been culled according to
                // their active viewports
                .addSystem<
                    SceneRenderExtractionSystem<
                        ViewportHandle,
                        GameObjectHandle,
                        NoCullingStrategy<GameObjectHandle>,
                        RenderCommandBuffer
                    >
                >(NoCullingStrategy<GameObjectHandle>()).addCommitPoint(CommitPoint::FlushCommands)

                 // Clear, bufferswapping
                .addPass<GameState>(GameState::Live)
                .addSystem<TransformClearSystem<GameObjectHandle>>()
                // WindowSizeUpdateSystem is not used right now:
                // it was mainly used for framebufefr resizing, which is now handled
                // directly in the GLFWPlatformManager
                //.addSystem<WindowSizeUpdateSystem<WindowHandle>>()
                //.addSystem<WindowSizeDirtyClearSystem<WindowHandle>>()
                .addSystem<SwapBuffersSystem<WindowHandle, PlatformCommandBuffer>>()
                .addSystem<GLFWWindowCloseSystem<WindowHandle, PlatformCommandBuffer>>()
                .addSystem<WindowBasedShutdownSystem<WindowHandle, PlatformCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Shutdown)
                .addSystem<DestroySessionSystem>()
            ;


    gameLoop.init(gameWorld.init());


    while (gameLoop.isRunning(gameWorld)) {

        framePacer.beginFrame();

        //app->eventManager().dispatchAll();
        //inputManager.poll(0.0f);

        // Game Logic Update
        const GamepadState gamepadState = GamepadState();// = {};//inputManager->gamepadState(Gamepad::ONE);
        const auto inputSnapshot = InputSnapshot(gamepadState);

        //const auto viewportSnapshots = renderTargetsRegistry.viewportSnapshots();

        // Frame Synchronization is now done via GLFWSwapBuffersSystems
        gameLoop.update(gameWorld, DELTA_TIME, inputSnapshot, {});//inputSnapshot, viewportSnapshots);;

        frameStats = framePacer.sync();;
        DELTA_TIME = frameStats.totalFrameTime;
    }


    return 0;
}