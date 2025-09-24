module;

#include <memory>
#include <string>

export module helios.glfw.application:GLFWFactory;

import :GLFWApplication;

export namespace helios::glfw::application {
    class GLFWFactory {

    public:

        static std::unique_ptr<GLFWApplication> makeOpenGLApp(std::string title);

        static GLFWWindowConfig makeWindowCfg(std::string title);
    };
}