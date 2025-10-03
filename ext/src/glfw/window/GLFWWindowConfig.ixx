module;

#include <GLFW/glfw3.h>

export module helios.ext.glfw.window.GLFWWindowConfig;

import helios.window.WindowConfig;

export namespace helios::ext::glfw::window {

    struct GLFWWindowConfig : public helios::window::WindowConfig {
        GLFWframebuffersizefun frameBufferSizeCallback = nullptr;
        ~GLFWWindowConfig() override = default;
    };


}