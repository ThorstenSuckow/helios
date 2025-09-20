module;

export module helios.glfw.window:GLFWWindowUserPointer;

import helios.platform.application.core;
import helios.rendering.core;
import helios.platform.window.core;

export namespace helios::glfw::window {

    struct GLFWWindowUserPointer {
        platform::application::core::Application* application;
        platform::window::core::Window* window;
    };

}
