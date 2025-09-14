#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


import helios.util;
import helios.platform.application.glfw;
import helios.rendering.opengl;
import helios.platform.input;
import helios.platform.input.glfw;
import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.window.core;
import helios.platform.window.glfw;

namespace heliosAppGlfw = helios::platform::application::glfw;
namespace heliosWinGlfw = helios::platform::window::glfw;
namespace heliosOpenGl = helios::rendering::opengl;
namespace heliosInput = helios::platform::input;
namespace heliosUtil = helios::util;

namespace heliosGLFWUtil = heliosUtil::glfw;

int main() {


    // 1. create the rendering device
    const auto opengl = std::make_unique<heliosOpenGl::OpenGLDevice>();
    // 2. create the config for the main window
    auto cfg = heliosGLFWUtil::GLFWFactory::makeWindowCfg("helios - Simple Cube Renderer");

    // 3. create the app.
    const auto app = heliosGLFWUtil::GLFWFactory::makeApplication(opengl.get());

    // 4. create the main window
    heliosWinGlfw::GLFWWindow& win = app->createWindow(cfg);

    // 5. initialize the app
    app->init();
    // ... and set focus to the window
    app->focus(win);

    // get the InputManager
    auto& inputManager = app->inputManager();

    while (!win.shouldClose()) {
        inputManager.tick(0.0f);

        if (inputManager.isKeyPressed(heliosInput::Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
        }

        win.swapBuffers();
    }

    return EXIT_SUCCESS;
}