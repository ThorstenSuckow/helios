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

// Helios Core Modules
import helios.math;

// Application Framework
import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;

// Input Management
import helios.input.InputManager;
import helios.input.types.Key;
import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadState;


// Rendering System
import helios.rendering.Renderable;
import helios.rendering.RenderQueue;
import helios.rendering.RenderPrototype;
import helios.rendering.RenderPassFactory;
import helios.rendering.Viewport;
import helios.rendering.ClearFlags;
import helios.rendering.model.Material;
import helios.rendering.model.config.MeshConfig;
import helios.rendering.model.config.MaterialProperties;
import helios.rendering.model.config.PrimitiveType;

import helios.rendering.asset.shape.basic.Triangle;
import helios.rendering.asset.shape.basic.Line;
import helios.rendering.asset.shape.basic.Grid;

// OpenGL Backend
import helios.ext.opengl.rendering.shader.OpenGLShader;
import helios.ext.opengl.rendering.model.OpenGLMesh;
import helios.ext.opengl.rendering.shader.OpenGLUniformLocationMap;
import helios.rendering.shader.UniformSemantics;
import helios.ext.opengl.rendering.OpenGLDevice;

// Scene Management
import helios.scene.Scene;
import helios.scene.SceneNode;
import helios.scene.CullNoneStrategy;
import helios.scene.Camera;
import helios.scene.CameraSceneNode;


// util
import helios.util.io.BasicStringFileReader;
import helios.util.time.Stopwatch;
import helios.util.log.LogSink;


// engine
import helios.engine.FramePacer;
import helios.engine.FrameStats;

// tooling
import helios.tooling.FpsMetrics;

// imgui
import helios.ext.imgui.ImGuiGlfwOpenGLBackend;
import helios.ext.imgui.ImGuiOverlay;
import helios.ext.imgui.ImGuiWidget;
import helios.ext.imgui.widgets.MainMenuWidget;
import helios.ext.imgui.widgets.FpsWidget;
import helios.ext.imgui.widgets.GamepadWidget;
import helios.ext.imgui.widgets.LogWidget;
import helios.ext.imgui.widgets.CameraWidget;
import helios.ext.imgui.ImGuiLogSink;

// Spaceship Widget
import helios.examples.spaceshipControl.SpaceshipWidget;

// game input handling, game objects
import helios.core.units;
import helios.engine.game.GameWorld;
import helios.engine.game.CommandBuffer;
import helios.engine.game.GameObject;
import helios.engine.game.InputSnapshot;
import helios.engine.game.UpdateContext;

// components, commands
import helios.engine.game.components.scene.SceneNodeComponent;
import helios.engine.game.components.input.TwinStickInputComponent;
import helios.engine.game.components.physics.Move2DComponent;
import helios.engine.game.components.physics.RotationStateComponent;
import helios.engine.game.components.physics.TranslationStateComponent;
import helios.engine.game.components.physics.HeadingComponent;
import helios.engine.game.components.physics.DirectionComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.ScaleComponent;

import helios.engine.game.systems.physics.Move2DSystem;
import helios.engine.game.systems.physics.ScaleSystem;
import helios.engine.game.systems.physics.HeadingSystem;
import helios.engine.game.systems.physics.ComposeTransformSystem;
import helios.engine.game.systems.scene.SceneSyncSystem;
import helios.engine.game.systems.post.TransformClearSystem;
import helios.engine.game.systems.post.ScaleClearSystem;

// ============================================================================
// Using Declarations
// ============================================================================

using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::model;
using namespace helios::rendering::model::config;
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
        "helios - Spaceship Control", 1980, 1024, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
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
    // Get native GLFW window
    // GLFWwindow* nativeWindow = win->nativeHandle();
    auto imguiBackend = helios::ext::imgui::ImGuiGlfwOpenGLBackend(win->nativeHandle());
    auto imguiOverlay = helios::ext::imgui::ImGuiOverlay::forBackend(&imguiBackend);
    auto fpsMetrics = helios::tooling::FpsMetrics();
    auto stopwatch = std::make_unique<helios::util::time::Stopwatch>();
    auto framePacer = helios::engine::FramePacer(std::move(stopwatch));
    // set target framerate
    framePacer.setTargetFps(0.0f);
    helios::engine::FrameStats frameStats{};
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

    // ========================================
    // 1.3 Logger Configuration
    // ========================================
    helios::util::log::LogManager::getInstance().enableLogging(true);
    auto imguiLogSink = std::make_shared<helios::ext::imgui::ImGuiLogSink>(logWidget);
    helios::util::log::LogManager::getInstance().enableSink(imguiLogSink);

    // ========================================
    // 2. Shader Creation
    // ========================================
    auto shader_ptr =
            std::make_shared<OpenGLShader>("./resources/cube.vert", "./resources/cube.frag", basicStringFileReader);

    // Map the ModelMatrix uniform to location 1 in the shader
    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    bool mapping = uniformLocationMap->set(UniformSemantics::ModelMatrix, 1);
    mapping = uniformLocationMap->set(UniformSemantics::ViewMatrix, 2);
    mapping = uniformLocationMap->set(UniformSemantics::ProjectionMatrix, 3);
    mapping = uniformLocationMap->set(UniformSemantics::MaterialBaseColor, 4);

    shader_ptr->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 3. Game Object Creation (Rendering)
    // ========================================

    // spaceship
    auto spaceshipMaterialProps = MaterialProperties(vec4(1.0f, 1.0f, 0.0f, 1.0f));
    auto spaceshipMaterialProps_shared = std::make_shared<MaterialProperties>(spaceshipMaterialProps);
    auto material_ptr = std::make_shared<Material>(shader_ptr, spaceshipMaterialProps_shared);

    // grid
    auto gridMaterialProps = MaterialProperties(vec4(0.0f, 1.0f, 1.0f, 0.2f));
    auto gridMaterialProps_shared = std::make_shared<MaterialProperties>(gridMaterialProps);
    auto gridMaterial_ptr = std::make_shared<Material>(shader_ptr, gridMaterialProps_shared);

    // pseudo gizmos
    auto leftStickGizmoMaterialProps = MaterialProperties();
    auto leftStickGizmoMaterialProps_shared = std::make_shared<MaterialProperties>(leftStickGizmoMaterialProps);
    auto leftStickGizmoMaterial_ptr = std::make_shared<Material>(shader_ptr, leftStickGizmoMaterialProps_shared);

    auto shipDirectionGizmoMaterialProps = leftStickGizmoMaterialProps.withBaseColor(vec4f(1.0f, 0.0f, 0.0f, 1.0f));
    auto shipDirectionGizmoMaterialProps_shared = std::make_shared<MaterialProperties>(shipDirectionGizmoMaterialProps);
    auto shipDirectionGizmoMaterial_ptr =
            std::make_shared<Material>(shader_ptr, shipDirectionGizmoMaterialProps_shared);


    // ========================================
    // 4. Mesh (Geometry) Creation
    // ========================================
    auto spaceship = Triangle{};
    auto leftStickGizmo = Line{};
    auto shipDirectionGizmo = Line{};
    auto grid = Grid{29, 19};

    // Configure the mesh for the spaceship to render as a line loop (wireframe)
    auto meshConfig = std::make_shared<const MeshConfig>(PrimitiveType::LineLoop);
    auto mesh_ptr = std::make_shared<OpenGLMesh>(spaceship, meshConfig);

    // configure the gizmos for controller input, as well as the grid rendering
    auto meshLineConfig = std::make_shared<const MeshConfig>(PrimitiveType::Lines);
    auto leftStickGizmoMesh = std::make_shared<OpenGLMesh>(leftStickGizmo, meshLineConfig);
    auto shipDirectionGizmoMesh = std::make_shared<OpenGLMesh>(shipDirectionGizmo, meshLineConfig);
    auto gridMesh_ptr = std::make_shared<OpenGLMesh>(grid, meshLineConfig);

    // ========================================
    // 5. Renderable and RenderPrototype
    // ========================================
    const auto renderPrototype = std::make_shared<RenderPrototype>(material_ptr, mesh_ptr);
    auto spaceshipRenderable = std::make_shared<Renderable>(renderPrototype);

    const auto leftStickGizmoPrototype =
            std::make_shared<RenderPrototype>(leftStickGizmoMaterial_ptr, leftStickGizmoMesh);
    auto leftStickGizmoRenderable = std::make_shared<Renderable>(leftStickGizmoPrototype);

    const auto shipDirectionGizmoPrototype =
            std::make_shared<RenderPrototype>(shipDirectionGizmoMaterial_ptr, shipDirectionGizmoMesh);
    auto shipDirectionGizmoRenderable = std::make_shared<Renderable>(shipDirectionGizmoPrototype);

    const auto gridPrototype = std::make_shared<RenderPrototype>(gridMaterial_ptr, gridMesh_ptr);
    auto gridRenderable = std::make_shared<Renderable>(gridPrototype);

    // ========================================
    // 6. Scene Graph Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));

    // Add the grid
    auto* gridSceneNode = scene->addNode(std::make_unique<helios::scene::SceneNode>(std::move(gridRenderable)));

    // Add the spaceship as a scene node
    auto* spaceshipSceneNode =
            scene->addNode(std::make_unique<helios::scene::SceneNode>(std::move(spaceshipRenderable)));
    spaceshipSceneNode->setTranslation(helios::math::vec3f{0.0f, 0.0f, 1.0f});

    // Add the gizmos
    auto* leftStickGizmoNode =
            spaceshipSceneNode->addNode(std::make_unique<helios::scene::SceneNode>(std::move(leftStickGizmoRenderable)));
    auto* shipDirectionGizmoNode =
            spaceshipSceneNode->addNode(std::make_unique<helios::scene::SceneNode>(std::move(shipDirectionGizmoRenderable)));
    leftStickGizmoNode->setInheritance(helios::math::TransformType::Translation);
    shipDirectionGizmoNode->setInheritance(helios::math::TransformType::Translation);


    // ========================================
    // 7. Register mainViewport-Camera w/ Setup
    // ========================================
    auto mainViewportCam = std::make_unique<helios::scene::Camera>();
    auto cameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(mainViewportCam));
    auto cameraSceneNode_ptr = cameraSceneNode.get();
    std::ignore = spaceshipSceneNode->addNode(std::move(cameraSceneNode));
    cameraSceneNode_ptr->setInheritance(
        helios::math::TransformType::Translation
    );
    mainViewport->setCameraSceneNode(cameraSceneNode_ptr);
    cameraSceneNode_ptr->setTranslation(
        helios::math::vec3f(0.0f, 0.0f, -(GRID_Y*CELL_SIZE / 2.0f)/tan(FOVY/2))
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
    // 9. Game-related Input-handling, GameWorld and GameObjects
    // ========================================

    auto gameWorld = helios::engine::game::GameWorld{};
    auto commandBuffer = helios::engine::game::CommandBuffer{};

    auto shipGameObject = std::make_unique<helios::engine::game::GameObject>();
    shipGameObject->add<helios::engine::game::components::scene::SceneNodeComponent>(spaceshipSceneNode);
    shipGameObject->add<helios::engine::game::components::input::TwinStickInputComponent>();
    shipGameObject->add<helios::engine::game::components::physics::Move2DComponent>();
    shipGameObject->add<helios::engine::game::components::physics::HeadingComponent>();
    shipGameObject->add<helios::engine::game::components::physics::DirectionComponent>();
    shipGameObject->add<helios::engine::game::components::physics::TransformComponent>();
    shipGameObject->add<helios::engine::game::components::physics::TranslationStateComponent>();
    shipGameObject->add<helios::engine::game::components::physics::RotationStateComponent>();
    shipGameObject->add<helios::engine::game::components::physics::ScaleComponent>(
        SPACESHIP_SIZE, SPACESHIP_SIZE, 0.0f, helios::core::units::Unit::Meter);
    auto* theShipPtr = gameWorld.addGameObject(std::move(shipGameObject));

    // Register the spaceship with the tuning widget
    spaceshipWidget->addGameObject("Player 1", theShipPtr);

    auto gridGameObject = std::make_unique<helios::engine::game::GameObject>();
    gridGameObject->add<helios::engine::game::components::scene::SceneNodeComponent>(gridSceneNode);
    gridGameObject->add<helios::engine::game::components::physics::TransformComponent>();
    gridGameObject->add<helios::engine::game::components::physics::ScaleComponent>(
        GRID_X * CELL_SIZE, GRID_Y * CELL_SIZE, 0.0f, helios::core::units::Unit::Meter
    );
    std::ignore = gameWorld.addGameObject(std::move(gridGameObject));

    // register the game systems
    gameWorld.addSystem<helios::engine::game::systems::physics::ScaleSystem>();
    gameWorld.addSystem<helios::engine::game::systems::physics::Move2DSystem>();
    gameWorld.addSystem<helios::engine::game::systems::physics::HeadingSystem>();
    gameWorld.addSystem<helios::engine::game::systems::physics::ComposeTransformSystem>();
    gameWorld.addSystem<helios::engine::game::systems::scene::SceneSyncSystem>(scene.get());
    gameWorld.addSystem<helios::engine::game::systems::post::TransformClearSystem>();
    gameWorld.addSystem<helios::engine::game::systems::post::ScaleClearSystem>();



    float DELTA_TIME = 0.0f;
    auto updateContext = helios::engine::game::UpdateContext{&commandBuffer, &gameWorld};

    // ========================================
    // 10. Main Game Loop
    // ========================================

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // ----------------------------------------
        // 10.1 Event and Input Processing
        // ----------------------------------------
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        // Check for ESC key to close the application
        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC] - Exiting..." << std::endl;
            win->setShouldClose(true);
        }

        // ----------------------------------------
        // 10.2 Game Logic Update
        // ----------------------------------------
        const GamepadState& gamepadState = inputManager.gamepadState(Gamepad::ONE);
        const auto inputSnapshot = helios::engine::game::InputSnapshot(gamepadState);

        updateContext.setDeltaTime(DELTA_TIME);
        updateContext.setInputSnapshot(&inputSnapshot);

        gameWorld.update(updateContext);
        commandBuffer.flush(gameWorld);



        // ----------------------------------------
        // 10.3 Gizmo / Debug Visualization Update
        // ----------------------------------------
        const auto* mc = theShipPtr->get<helios::engine::game::components::physics::Move2DComponent>();
        if (mc) {
            leftStickGizmoNode->setScale((mc->steeringInput() * mc->throttle()  * 4.0f).toVec3());
            shipDirectionGizmoNode->setScale(mc->velocity().normalize() * mc->speedRatio() * 4.0f);
        }

        // ----------------------------------------
        // 10.4 Rendering
        // ----------------------------------------
        const auto& snapshot = scene->createSnapshot(mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        // ========================================
        // ImGui Rendering
        // ========================================
        imguiOverlay.render();

        // ----------------------------------------
        // 10.5 Frame Synchronization
        // ----------------------------------------
        // swap time / idle time should be read out here
        win->swapBuffers();

        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        DELTA_TIME = frameStats.totalFrameTime;
    }

    return EXIT_SUCCESS;
}
