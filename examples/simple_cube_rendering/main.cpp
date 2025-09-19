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

    auto glfwGuard = heliosAppGlfw::GLFWRAIIGuard();

    // 1. create the rendering device
    const auto opengl = std::make_unique<heliosOpenGl::OpenGLDevice>();

    // 2. create the config for the main window
    auto cfg = heliosGLFWUtil::GLFWFactory::makeWindowCfg(
        "helios - Simple Cube Renderer");

    // 3. create the app.
    const auto app = heliosGLFWUtil::GLFWFactory::makeApplication(
        opengl.get()
    );

    // 4. initialize the app
    app->init();

    // 5. create the main window and focus it
    heliosWinGlfw::GLFWWindow& win = app->createWindow(cfg);

    win.setWindowUserPointer(std::make_unique<heliosWinGlfw::GLFWWindowUserPointer>(
        opengl.get(), &win
    ));

    win.setFramebufferSizeCallback([] (GLFWwindow* nativeWin, const int width, const int height) {
        if (const auto* ptr = static_cast<heliosWinGlfw::GLFWWindowUserPointer*>(glfwGetWindowUserPointer(nativeWin))) {
            auto* win = dynamic_cast<heliosWinGlfw::GLFWWindow*>(ptr->window);
            const auto& viewport = win->viewport();
            std::cout << width << height;
            ptr->renderingDevice->setViewport(viewport[0], viewport[1], width, height);
        }
    });


    // ... and set focus to the window
    app->focus(win);

    // get the InputManager
    heliosInput::InputManager& inputManager = app->inputManager();

    while (!win.shouldClose()) {
        inputManager.tick(0.0f);

        if (inputManager.isKeyPressed(heliosInput::Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            std::ignore = win.setShouldClose(true);
        }

        win.swapBuffers();
    }

    return EXIT_SUCCESS;
}