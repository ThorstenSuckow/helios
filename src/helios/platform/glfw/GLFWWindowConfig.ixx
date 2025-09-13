module;

#include <GLFW/glfw3.h>

export module helios.platform.glfw:GLFWWindowConfig;

import helios.platform.core;

export namespace helios::platform::glfw {

    struct GLFWWindowConfig : public WindowConfig {
        GLFWframebuffersizefun frameBufferSizeCallback = nullptr;
        ~GLFWWindowConfig() override = default;
    };


}