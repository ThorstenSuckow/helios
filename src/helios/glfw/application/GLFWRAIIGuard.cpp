module;

#include <GLFW/glfw3.h>
#include <stdexcept>

module helios.glfw.application;


namespace helios::glfw::application {
    GLFWRAIIGuard::GLFWRAIIGuard() {
        if (glfwInit() == GLFW_FALSE) {
            throw std::runtime_error("Failed to initialize GLFW");
        }
    }

    GLFWRAIIGuard::~GLFWRAIIGuard() {
        glfwTerminate();
    }
}