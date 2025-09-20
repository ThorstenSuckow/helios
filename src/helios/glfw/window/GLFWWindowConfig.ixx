module;

#include <GLFW/glfw3.h>

export module helios.glfw.window:GLFWWindowConfig;

import helios.platform.window.core;

using namespace helios::platform::window::core;

export namespace helios::glfw::window {

    struct GLFWWindowConfig : public WindowConfig {
        GLFWframebuffersizefun frameBufferSizeCallback = nullptr;
        ~GLFWWindowConfig() override = default;
    };


}