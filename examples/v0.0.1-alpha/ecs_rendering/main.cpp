import helios;
import helios.ext;

import helios.examples.ecs_rendering.GameLoop;

#include "Namespaces.h"
#include <memory>

int main() {

    // ========================================
    // Constants
    // ========================================
    constexpr unsigned int SCREEN_WIDTH  = 1980;
    constexpr unsigned int SCREEN_HEIGHT = 1080;

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
    auto deadzoneStrategy = std::make_unique<RadialDeadzoneStrategy>();
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
    gameWorld.registerManager<GLFWPlatformManager<WindowHandle,  /*InputHandle, */ StateCommandBuffer, EngineCommandBuffer>>();
    gameWorld.registerManager<OpenGLShaderCompileManager<ShaderHandle>>(gameWorld.renderResourceWorld());
    //gameWorld.registerManager<WarmupManager<OpenGLShaderSourcePool>>(shaderSourcePool);


    // ========================================
    // Window Setup
    // ========================================
    auto mainWindow = gameWorld.add<WindowHandle>(WindowId("mainWindow"));
    mainWindow.setActive(true);
    mainWindow.add<WindowCreateRequestComponent<WindowHandle>>(WindowConfig{
        "helios - ECS Rendering Demo",
        {SCREEN_WIDTH, SCREEN_HEIGHT},
        ASPECT_RATIO_NUMER,
        ASPECT_RATIO_DENOM
    });

    
    // ========================================
    // Scene and Viewport Setup
    // ========================================
    //auto win = dynamic_cast<GLFWWindow*>(app->current());

    //auto  MainSceneHandle = sceneHandleRegistry.getOrCreate("mainScene");
    //scenePool.add(Scene, "MainScene");

    /*auto [WindowFrameBufferHandle, MainViewportHandle] = gameWorld.bind(
        Framebuffer(), FramebufferId("WindowFramebuffer"),
        Viewport(0.0f, 0.0f, 1.0f, 1.0f), ViewportId("MainViewport")
    );


    renderTargets.resolve(MainViewportHandle)
                  ->setClearFlags(std::to_underlying(ClearFlags::Color))
                   .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
*/


    // ========================================
    // Rendering Management setup
    // ========================================

    // shader
    auto cubeShader = gameWorld.add<ShaderHandle>(ShaderId("CubeShader"));
    cubeShader.add<ShaderSourceComponent<ShaderHandle>>(
    "./resources/cube.vert", "./resources/cube.frag"
    );

    auto cubeMesh = gameWorld.add<MeshHandle>(MeshId("CubeMesh"));




   /* auto uniformLocationMap = OpenGLUniformLocationMap();
    uniformLocationMap.set(UniformSemantics::ModelMatrix, 1);
    cubeShader.setUniformLocationMap(uniformLocationMap);
    auto CubeShaderHandle = renderResources.add(std::move(cubeShader), ShaderId("CubeShader"));

    // mesh
    auto cubeMesh       = Cube{};
    auto CubeMeshHandle = renderResources.add(MeshData{*cubeMesh.vertices, *cubeMesh.indices}, MeshId("CubeMesh"));
    
    // material
    auto CubeMaterialHandle = renderResources.add(MaterialProperties{helios::util::Colors::Purple, 0.0f}, MaterialId("CubeMaterial"));
*/
    // ========================================
    // Entity Setup
    // ========================================
    /*auto camera = gameWorld.addGameObject();
    auto pcc = camera.add<PerspectiveCameraComponent>();
    pcc.setPerspective(FOVY, ASPECT_RATIO_NUMER / ASPECT_RATIO_DENOM, 0.1f, 1000.0f);
    auto tsc = camera.add<TranslationStateComponent>();
    tsc.setTranslation(vec3f(0.0f, 0.0f, -100.0f));
    auto lac = camera.add<LookAtComponent>();
    lac.lookAt(vec3f(0.0f, 0.0f, 0.0f), vec3f(0.0f, 1.0f, 0.0f));

    // camera is viewing the main scene
    camera.add<SceneMemberComponent>(MainSceneHandle);

    // cube
    auto cube = gameWorld.addGameObject();
    cube.add<SceneMemberComponent>(MainSceneHandle);
    cube.add<DimensionComponent>(2.0f, 2.0f, 2.0f);
    cube.add<RenderPrototypeComponent>(CubeMeshHandle, CubeShaderHandle, CubeMaterialHandle);
    */

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

    auto stopwatch = std::make_unique<Stopwatch>();
    auto framePacer = FramePacer(std::move(stopwatch));
    framePacer.setTargetFps(0.0f);
    FrameStats frameStats{};

    gameLoop.init(gameWorld.init());

    // ----------------------------------------
    // GameLoop Config
    // ----------------------------------------
    gameLoop.phase(PhaseType::Pre)
                .addPass<GameState>(GameState::Any)
                .addSystem<GameFlowSystem<StateCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Booting)
                .addSystem<PlatformInitSystem<EngineCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Booted | GameState::Live)
                .addSystem<PollEventsSystem<EngineCommandBuffer>>()
                .addSystem<WindowCreateSystem<WindowHandle, EngineCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Warmup)
                .addSystem<ShaderCompileSystem<ShaderHandle, RenderCommandBuffer>>()
                .addSystem<WarmupDoneSystem<ShaderHandle, StateCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Live)
                .addSystem<ScaleSystem<GameObjectHandle>>();

            gameLoop.phase(PhaseType::Main)
                .addPass(GameState::Live);

            gameLoop.phase(PhaseType::Post)
                 .addPass(GameState::Live)

                 //.addSystem<LocalComposeTransformSystem>()
                 //.addSystem<WorldTransformSystem>()

                // this will produce render commands after scenes have been culled according to
                // their active viewports
                //.addSystem<SceneMemberRenderExtractionSystem<NoCullingStrategy>>()
                .addCommitPoint(CommitPoint::FlushCommands)

                 // Clear, bufferswapping
                .addPass<GameState>(GameState::Live)
                .addSystem<TransformClearSystem<GameObjectHandle>>()
                .addSystem<SwapBuffersSystem<WindowHandle, EngineCommandBuffer>>()
                .addSystem<GLFWWindowCloseSystem<WindowHandle, EngineCommandBuffer>>()
                .addSystem<WindowBasedShutdownSystem<WindowHandle, EngineCommandBuffer>>()
                .addCommitPoint(CommitPoint::Structural)

                .addPass<GameState>(GameState::Shutdown)
                .addSystem<DestroySessionSystem>()
            ;


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