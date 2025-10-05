module;

export module helios.ext.glfw.window.GLFWWindowUserPointer;

import helios.app.Application;
import helios.window.Window;

export namespace helios::ext::glfw::window {

    struct GLFWWindowUserPointer {
        helios::app::Application* application;
        helios::window::Window* window;
    };

}
