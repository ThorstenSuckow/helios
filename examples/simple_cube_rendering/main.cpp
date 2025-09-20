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


int main() {

    auto glfwGuard = helios::glfw::application::GLFWRAIIGuard();

    const auto app = helios::glfw::application::GLFWFactory::makeOpenGLApp(
        "helios - Simple Cube Renderer"
    );

    auto win = dynamic_cast<helios::glfw::window::GLFWWindow*>(app->current());


    // get the InputManager
    helios::platform::input::InputManager& inputManager = app->inputManager();
    while (!win->shouldClose()) {
        inputManager.tick(0.0f);

        if (inputManager.isKeyPressed(helios::platform::input::Key::ESC)) {
            std::cout << "Key Pressed [ESC]" << std::endl;
            std::ignore = win->setShouldClose(true);
        }

        win->swapBuffers();
    }

    return EXIT_SUCCESS;
}