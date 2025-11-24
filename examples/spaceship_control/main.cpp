#include <cstdlib>
#include <iostream>

// Helios Core Modules
import helios.math.types;
import helios.math;

// Application Framework
import helios.ext.glfw.app.GLFWFactory;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;

// Input Management
import helios.input.InputManager;
import helios.input.types.Key;

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

// File I/O
import helios.util.io.BasicStringFileReader;

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


using namespace helios::ext::glfw::app;
using namespace helios::rendering;
using namespace helios::rendering::model;
using namespace helios::rendering::model::config;
using namespace helios::rendering::shader;
using namespace helios::ext::opengl::rendering;
using namespace helios::ext::opengl::rendering::shader;
using namespace helios::ext::opengl::rendering::model;
using namespace helios::rendering::asset::shape::basic;
using namespace helios::input::types;
using namespace helios::ext::glfw::window;
using namespace helios::util::io;
using namespace helios::scene;
using namespace helios::math;


int main() {

    // ========================================
    // 1. Application and Window Setup
    // ========================================

    const auto app = GLFWFactory::makeOpenGLApp(
        "helios - Spaceship Control"
    );

    auto win = dynamic_cast<GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    auto mainViewportCam = std::make_shared<helios::scene::Camera>();
    mainViewport->setCamera(mainViewportCam)
                .setClearFlags(std::to_underlying(ClearFlags::Color))
                .setClearColor(vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->renderTarget().addViewport(mainViewport);

    // Get the InputManager for handling keyboard input
    helios::input::InputManager& inputManager = app->inputManager();

    const auto basicStringFileReader = BasicStringFileReader();


    // ========================================
    // 2. Shader Creation
    // ========================================
    auto shader_ptr = std::make_shared<OpenGLShader>(
        "./resources/cube.vert",
        "./resources/cube.frag",
        basicStringFileReader
    );


    // Map the WorldMatrix uniform to location 1 in the shader
    auto uniformLocationMap = std::make_unique<OpenGLUniformLocationMap>();
    [[maybe_unused]] bool mappingSuccess = uniformLocationMap->set(UniformSemantics::WorldMatrix, 1);
    shader_ptr->setUniformLocationMap(std::move(uniformLocationMap));

    // ========================================
    // 3. Spaceship Creation
    // ========================================
    auto spaceshipMaterialProps = MaterialProperties();
    auto spaceshipMaterialProps_shared = std::make_shared<MaterialProperties>(spaceshipMaterialProps);
    auto material_ptr = std::make_shared<Material>(
        shader_ptr, spaceshipMaterialProps_shared
    );

    // ========================================
    // 4. Mesh (Geometry) Creation
    // ========================================
    auto spaceship = Triangle{};

    // Configure the mesh to render as a line loop (wireframe)
    auto meshConfig = std::make_shared<const MeshConfig>(
        PrimitiveType::LineLoop
    );
    auto mesh_ptr = std::make_shared<OpenGLMesh>(spaceship, meshConfig);

    // ========================================
    // 5. Renderable and RenderPrototype
    // ========================================
    const auto renderPrototype = std::make_shared<RenderPrototype>(
        material_ptr, mesh_ptr
    );
    auto spaceshipRenderable = std::make_shared<Renderable>(renderPrototype);

    // ========================================
    // 6. Scene Graph Setup
    // ========================================
    auto frustumCullingStrategy = std::make_unique<CullNoneStrategy>();
    auto scene = std::make_unique<helios::scene::Scene>(std::move(frustumCullingStrategy));

    // Add the spaceship as a scene node
    auto* spaceshipSceneNode = scene->addNode(std::make_unique<helios::scene::SceneNode>(std::move(spaceshipRenderable)));

    // Scale the spacehip to half its original size
    spaceshipSceneNode->scale(vec3f(0.25f, 0.25f, 0.25f));

    // ========================================
    // 7. Register mainViewport-Camera w/ Setup
    // ========================================
    std::ignore = scene->addNode(std::make_unique<CameraSceneNode>(mainViewportCam));


    // ========================================
    // 8. Main Render Loop
    // ========================================
    float degrees = 0.0f;
    float rad = 0.0f;
    constexpr float rotationSpeed = 2.25f; // Degrees per frame


    while (!win->shouldClose()) {
        // Process window and input events
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        // Check for ESC key to close the application
        if (inputManager.isKeyPressed(Key::ESC)) {
            std::cout << "Key Pressed [ESC] - Exiting..." << std::endl;
            win->setShouldClose(true);
        }

        // Update rotation angle
        degrees += rotationSpeed;
        if (degrees >= 360.0f) {
            degrees -= 360.0f;
        }
        rad = helios::math::radians(degrees);

        // Rotate the spacehip around an arbitrary axis
        spaceshipSceneNode->rotate(helios::math::rotate(
            helios::math::mat4f::identity(),
            rad,
            helios::math::vec3f(0.0f, 0.0f, 1.0f) // Rotation axis
        ));

        // Create a snapshot of the scene and render it
        const auto& snapshot = scene->createSnapshot(mainViewport);
        if (snapshot.has_value()) {
            auto renderPass = RenderPassFactory::getInstance().buildRenderPass(*snapshot);
            app->renderingDevice().render(renderPass);
        }

        // Swap front and back buffers
        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}