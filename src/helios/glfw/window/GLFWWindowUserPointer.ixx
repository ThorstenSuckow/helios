module;

export module helios.glfw.window:GLFWWindowUserPointer;

import helios.rendering.core;
import helios.platform.window.core;

export namespace helios::glfw::window {

    struct GLFWWindowUserPointer {
        rendering::core::RenderingDevice* renderingDevice;
        platform::window::core::Window* window;
    };

}
