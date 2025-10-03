module;

#include <memory>
#include <string>

export module helios.ext.glfw.app.GLFWFactory;

import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindowConfig;

export namespace helios::ext::glfw::app {
    class GLFWFactory {

    public:

        static std::unique_ptr<GLFWApplication> makeOpenGLApp(std::string title);

        static helios::ext::glfw::window::GLFWWindowConfig makeWindowCfg(std::string title);
    };
}