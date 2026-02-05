// ============================================================================
// Text Rendering Demo
// ============================================================================
//
// This example demonstrates dynamic text rendering using the helios text
// rendering system. It showcases:
// - FreeType-based glyph rendering with OpenGL
// - Font loading and configuration
// - Orthographic projection for 2D UI text
// - Real-time text updates (displaying frame time)
//
// ============================================================================

// ============================================================================
// Includes
// ============================================================================

#include <iostream>
#include <memory>
#include <string>

import helios.ext;
import helios;


int main() {

    // ========================================================================
    // 1. Constants
    // ========================================================================
    constexpr int SCREEN_WIDTH = 1920;
    constexpr int SCREEN_HEIGHT = 1080;
    constexpr float ASPECT_RATIO_NUMER = 16.0f;
    constexpr float ASPECT_RATIO_DENOM = 9.0f;

    // ========================================================================
    // 2. Application and Window Setup
    // ========================================================================
    // Create an OpenGL application with GLFW window management.
    const auto app = helios::ext::glfw::app::GLFWFactory::makeOpenGLApp(
        "helios - Text Rendering Demo",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto* win = dynamic_cast<helios::ext::glfw::window::GLFWWindow*>(app->current());

    // Create the main viewport covering the full window.
    auto mainViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
    mainViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Color))
                .setClearColor(helios::math::vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    // Disable logging for cleaner console output.
    helios::util::log::LogManager::getInstance().enableLogging(false);

    // ========================================================================
    // 3. Frame Timing Setup
    // ========================================================================
    // Configure frame pacing for consistent timing (0 = unlimited FPS).
    auto fpsMetrics = helios::engine::tooling::FpsMetrics();
    auto framePacer = helios::engine::tooling::FramePacer(
        std::make_unique<helios::util::time::Stopwatch>()
    );
    framePacer.setTargetFps(0.0f);
    helios::engine::tooling::FrameStats frameStats{};

    // ========================================================================
    // 4. UI Scene and Camera Setup
    // ========================================================================
    // Create a dedicated scene for UI elements with no culling.
    auto uiScene = std::make_unique<helios::scene::Scene>(
        std::make_unique<helios::scene::CullNoneStrategy>()
    );

    // Create a viewport for UI rendering.
    auto uiViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);
    uiViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Color));
    win->addViewport(uiViewport);

    // Configure orthographic camera for 2D text rendering.
    // Origin (0,0) is at the bottom-left corner of the screen.
    auto uiCamera = std::make_unique<helios::scene::Camera>();
    auto uiCameraSceneNode = std::make_unique<helios::scene::CameraSceneNode>(std::move(uiCamera));
    auto* uiCameraSceneNode_ptr = uiCameraSceneNode.get();

    uiCameraSceneNode_ptr->setInheritance(helios::math::TransformType::Translation);
    uiViewport->setCameraSceneNode(uiCameraSceneNode_ptr);
    uiCameraSceneNode_ptr->setTranslation(helios::math::vec3f(0.0f, 0.0f, -100.0f));
    uiCameraSceneNode_ptr->camera().setOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
    uiCameraSceneNode_ptr->lookAtLocal(
        helios::math::vec3f(0.0f, 0.0f, 0.0f),
        helios::math::vec3f(0.0f, 1.0f, 0.0f)
    );

    std::ignore = uiScene->addNode(std::move(uiCameraSceneNode));

    // ========================================================================
    // 5. Text Shader Setup
    // ========================================================================
    // Configure uniform locations for the text shader.
    auto glyphUniformLocationMap = std::make_unique<
        helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap
    >();
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, 1);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextTexture, 3);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextColor, 4);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ModelMatrix, 8);
    glyphUniformLocationMap->set(helios::rendering::shader::UniformSemantics::ViewMatrix, 9);

    // Create the text shader from vertex and fragment shader files.
    auto glyphShader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
        "resources/font_shader.vert",
        "resources/font_shader.frag",
        helios::util::io::BasicStringFileReader()
    );
    glyphShader->setUniformLocationMap(std::move(glyphUniformLocationMap));

    // ========================================================================
    // 6. Font Loading
    // ========================================================================
    // Load the font via the rendering device's font resource provider.
    auto fontId = helios::engine::core::data::FontId{"roboto"};
    auto& fontResourceProvider = app->renderingDevice().fontResourceProvider();
    fontResourceProvider.loadFont(fontId, 24, "resources/Roboto-SemiBoldItalic.ttf");

    // ========================================================================
    // 7. Text Prototype and Renderable Creation
    // ========================================================================
    // Create a reusable text prototype with shader and font configuration.
    auto uiTextPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
        glyphShader,
        std::make_shared<helios::rendering::text::TextShaderProperties>(),
        &fontResourceProvider
    );

    // Create a TextRenderable for displaying dynamic text.
    auto textRenderable = std::make_shared<helios::rendering::text::TextRenderable>(
        std::make_unique<helios::rendering::text::TextMesh>("Hello World!", 1.0f, fontId),
        uiTextPrototype
    );

    // Add the text to the UI scene graph.
    auto textSceneNode = std::make_unique<helios::scene::SceneNode>(textRenderable);
    auto* textNode = uiScene->addNode(std::move(textSceneNode));
    textNode->setScale({1.0f, 1.0f, 1.0f});
    textNode->setTranslation({100.0f, 100.0f, 0.0f});

    // ========================================================================
    // 8. Input Setup
    // ========================================================================
    helios::input::InputManager& inputManager = app->inputManager();

    // ========================================================================
    // 9. Main Loop
    // ========================================================================
    float deltaTime = 0.0f;

    while (!win->shouldClose()) {
        framePacer.beginFrame();

        // Process window events and poll input.
        app->eventManager().dispatchAll();
        inputManager.poll(0.0f);

        // Exit on ESC key.
        if (inputManager.isKeyPressed(helios::input::types::Key::ESC)) {
            win->setShouldClose(true);
        }

        // Update text content with current frame time.
        textRenderable->setText(std::format("Frame Time: {:.4f} ms", deltaTime * 1000.0f));

        // Render the UI scene.
        const auto& uiSnapshot = uiScene->createSnapshot(*uiViewport);
        if (uiSnapshot.has_value()) {
            auto uiRenderPass = helios::rendering::RenderPassFactory::getInstance()
                .buildRenderPass(*uiSnapshot);
            app->renderingDevice().render(uiRenderPass);
        }

        win->swapBuffers();

        // Update frame timing.
        frameStats = framePacer.sync();
        fpsMetrics.addFrame(frameStats);
        deltaTime = frameStats.totalFrameTime;
    }


    return EXIT_SUCCESS;
}