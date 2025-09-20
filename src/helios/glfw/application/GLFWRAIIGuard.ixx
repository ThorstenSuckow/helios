module;

#include <GLFW/glfw3.h>
#include <stdexcept>

export module helios.glfw.application:GLFWRAIIGuard;


export namespace helios::glfw::application {

    class GLFWRAIIGuard {

    public:
        GLFWRAIIGuard() {
            if (glfwInit() == GLFW_FALSE) {
                throw std::runtime_error("Failed to initialize GLFW");
            }
        }

        ~GLFWRAIIGuard() {
            glfwTerminate();
        }

        GLFWRAIIGuard(const GLFWRAIIGuard&) = delete;

        GLFWRAIIGuard operator=(const GLFWRAIIGuard&) = delete;
    };

}


