#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


import helios.platform.glfw;
import helios.rendering.opengl;
import helios.platform.input;
import helios.platform.input.glfw;
import helios.platform.input.core;
import helios.platform.input.types;

namespace heliosGlfw = helios::platform::glfw;
namespace heliosOpenGl = helios::rendering::opengl;
namespace heliosInput = helios::platform::input;

int main() {

    const auto opengl = std::make_unique<heliosOpenGl::OpenGLDevice>();

    const auto app = std::make_unique<heliosGlfw::GLFWApplication>(opengl.get());
    auto cfg = heliosGlfw::GLFWWindowConfig{};
    cfg.title = "helios - Simple Cube Renderer";
    cfg.frameBufferSizeCallback = [](GLFWwindow* win, const int width, const int height) {
        glViewport(0, 0, width, height);
    };

    heliosGlfw::GLFWWindow& win = app->createWindow(cfg);
    app->init();

    auto glfwInput = std::make_unique<heliosInput::glfw::GLFWInput>();
    std::unique_ptr<heliosInput::core::InputAdapter> input = std::move(glfwInput);
    heliosInput::InputManager inputManager{std::move(input)};


    while (!win.shouldClose()) {

        if (inputManager.isKeyPressed(heliosInput::Key::ESC, win)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
        }

        win.swapBuffers().pollEvents();
    }



    return EXIT_SUCCESS;
}