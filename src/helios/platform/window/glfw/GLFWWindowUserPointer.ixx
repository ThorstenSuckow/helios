module;

export module helios.platform.window.glfw:GLFWWindowUserPointer;

import helios.rendering.core;
import helios.platform.window.core;

export namespace helios::platform::window::glfw {

    struct GLFWWindowUserPointer {
        rendering::core::RenderingDevice* renderingDevice;
        core::Window* window;
    };

}
