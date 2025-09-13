module;

#include <GLFW/glfw3.h>

export module helios.platform.window.glfw:GLFWWindowConfig;

import helios.platform.window.core;

using namespace helios::platform::window::core;

export namespace helios::platform::window::glfw {

    struct GLFWWindowConfig : public WindowConfig {
        GLFWframebuffersizefun frameBufferSizeCallback = nullptr;
        ~GLFWWindowConfig() override = default;
    };


}