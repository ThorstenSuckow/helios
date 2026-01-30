// ============================================================================
// Includes
// ============================================================================

#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <vector>


import helios.ext;
import helios;

#define ASPECT_RATIO_NUMER 4
#define ASPECT_RATIO_DENOM 3




int main() {


    // ========================================
    // Window Setup
    // ========================================
    const auto app = helios::ext::glfw::app::GLFWFactory::makeOpenGLApp(
        "helios - Text Rendering Demo", 800, 600, ASPECT_RATIO_NUMER, ASPECT_RATIO_DENOM
    );

    auto win = dynamic_cast<helios::ext::glfw::window::GLFWWindow*>(app->current());
    auto mainViewport = std::make_shared<helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    mainViewport->setClearFlags(std::to_underlying(helios::rendering::ClearFlags::Color))
                  .setClearColor(helios::math::vec4f(0.051f, 0.051f, 0.153f, 1.0f));
    win->addViewport(mainViewport);

    helios::input::InputManager& inputManager = app->inputManager();

    // ========================================
    // Freetype
    // ========================================



    // use orthographic projection. The top parameter could be set to the original window's
    // height, this should be bound to the framebuffer's size the rendering oocurs later on.
    // (0.0f, 0.0f) refers to the bottom left corner of the screen.
    helios::math::mat4f projection = helios::math::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0);



    auto uniformLocationMap = std::make_unique<helios::ext::opengl::rendering::shader::OpenGLUniformLocationMap>();
    auto success = uniformLocationMap->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, 1);
    success = uniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextTexture, 3);
    success = uniformLocationMap->set(helios::rendering::shader::UniformSemantics::TextColor, 4);

    auto shader = std::make_shared<helios::ext::opengl::rendering::shader::OpenGLShader>(
        "resources/font_shader.vert",
        "resources/font_shader.frag",
        helios::util::io::BasicStringFileReader()
    );

    shader->setUniformLocationMap(std::move(uniformLocationMap));


    // define global text properties (arial)


    auto WorkbenchFont = helios::engine::core::data::FontId{"wokbench"};

    // create reusable text prototype with specific shader using the global text properties.
    auto textPrototype = std::make_shared<helios::rendering::text::TextRenderPrototype>(
        shader, std::make_shared<helios::rendering::text::config::TextShaderProperties>()
    );

    // create a unique TextRenderable that renders the string "Hello World!"
    auto textRenderable = std::make_unique<helios::rendering::text::TextRenderable>(
        "Hello World",
        textPrototype,
        helios::rendering::text::DrawProperties{
            .fontId=WorkbenchFont,
            .position=helios::math::vec2f{50.0f, 50.0f}
        },
        helios::rendering::text::config::TextShaderPropertiesOverride{
            .baseColor=helios::util::Colors::Pink
        }
    );


    auto& openGlDevice = app->renderingDevice();
    openGlDevice.textRenderer().addFontFamily(
        WorkbenchFont, "resources/Roboto-SemiBoldItalic.ttf");


    // ========================================
    // Main Loop
    // ========================================
    unsigned int i=0;
    while (!win->shouldClose()) {

        inputManager.poll(0.0f);

        // Check for ESC key to close the application
        if (inputManager.isKeyPressed(helios::input::types::Key::ESC)) {
            win->setShouldClose(true);
        }

        auto frameUniformValues = std::make_unique<helios::rendering::shader::UniformValueMap>();
        frameUniformValues->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, projection);
        frameUniformValues->set(helios::rendering::shader::UniformSemantics::TextTexture, helios::ext::opengl::rendering::OpenGLGlyphTextRenderer::textTextureUnit());

        auto uniformValues = helios::rendering::shader::UniformValueMap();
        textRenderable->setText(std::format("Hello World {0}", i++));

        // manually create command updates
        textRenderable->writeUniformValues(uniformValues);

        auto uiRenderQueue = std::make_unique<helios::rendering::RenderQueue>();

        auto textRenderCommand = helios::rendering::text::TextRenderCommand(
            std::string{textRenderable->text()},
            textPrototype.get(),
            textRenderable->drawProperties(),
            uniformValues
        );

        uiRenderQueue->add(std::move(textRenderCommand));
        auto uiRenderPass = helios::rendering::RenderPass(mainViewport, std::move(uiRenderQueue));
        uiRenderPass.setFrameUniformValues(std::move(frameUniformValues));


        openGlDevice.render(uiRenderPass);

        win->swapBuffers();
    }



    return 0;
}