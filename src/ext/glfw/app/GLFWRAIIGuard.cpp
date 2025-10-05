module;

#include <GLFW/glfw3.h>
#include <stdexcept>

module helios.ext.glfw.app.GLFWRAIIGuard;


namespace helios::ext::glfw::app {
    GLFWRAIIGuard::GLFWRAIIGuard() {
        if (glfwInit() == GLFW_FALSE) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    GLFWRAIIGuard::~GLFWRAIIGuard() {
        glfwTerminate();
    }
}