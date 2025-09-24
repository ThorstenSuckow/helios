module;

#include <GLFW/glfw3.h>
#include <stdexcept>

export module helios.glfw.application:GLFWRAIIGuard;


export namespace helios::glfw::application {

    class GLFWRAIIGuard {

    public:
        GLFWRAIIGuard();

        ~GLFWRAIIGuard();

        GLFWRAIIGuard(const GLFWRAIIGuard&) = delete;

        GLFWRAIIGuard operator=(const GLFWRAIIGuard&) = delete;
    };

}


