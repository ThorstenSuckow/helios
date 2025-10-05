module;

#include <GLFW/glfw3.h>

export module helios.ext.glfw.window.GLFWWindowConfig;

import helios.window.WindowConfig;

export namespace helios::ext::glfw::window {

    /**
     * Configuration structure for a GLFWWindow.
     *
     * Extends helios::window::WindowConfig by adding a `GLFWframebuffersizefun`,
     * which represents a C-style callback for framebuffer size events.
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#gae18026e294dde685ed2e5f759533144d
     */
    struct GLFWWindowConfig : public helios::window::WindowConfig {
        /**
         *The GLFW framebuffer size callback.
         */
        GLFWframebuffersizefun frameBufferSizeCallback = nullptr;
        ~GLFWWindowConfig() override = default;
    };


}