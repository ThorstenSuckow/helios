#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <cstdlib>


import helios.util;
import helios.glfw.application;
import helios.rendering.opengl;
import helios.platform.input;
import helios.glfw.input;
import helios.platform.input.core;
import helios.platform.input.types;
import helios.platform.window.core;
import helios.glfw.window;

namespace heliosAppGlfw = helios::glfw::application;
namespace heliosWinGlfw = helios::glfw::window;
namespace heliosOpenGl = helios::rendering::opengl;
namespace heliosInput = helios::platform::input;
namespace heliosUtil = helios::util;

int main() {

    auto glfwGuard = heliosAppGlfw::GLFWRAIIGuard();

    // 1. create the rendering device
    const auto opengl = std::make_unique<heliosOpenGl::OpenGLDevice>();

    // 2. create the config for the main window
    auto cfg = heliosAppGlfw::GLFWFactory::makeWindowCfg(
        "helios - Simple Cube Renderer");

    // 3. create the app.
    const auto app = heliosAppGlfw::GLFWFactory::makeApplication(
        opengl.get()
    );

    // 4. initialize the app
    app->init();

    // 5. create the main window and focus it
    heliosWinGlfw::GLFWWindow& win = app->createWindow(cfg);

    // [OPTIONAL] 5.1 set the window user pointer and register the framebuffersizecallback
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


    // 6. set focus to the window
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