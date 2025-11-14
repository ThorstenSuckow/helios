/**
 * @file GLFWWindowUserPointer.ixx
 * @brief Helper storing user pointer data for GLFW windows.
 */
module;

export module helios.ext.glfw.window.GLFWWindowUserPointer;

import helios.app.Application;
import helios.window.Window;

export namespace helios::ext::glfw::window {

    /**
     * @brief A structure to hold user defined data associated with a GLFWwindow.
     *
     * This struct is intended to be used with `glfwSetWindowUserPointer` to allow
     * GLFW callbacks access data held within this struct.
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga3d2fc6026e690ab31a13f78bc9fd3651
     */
    struct GLFWWindowUserPointer {
        /**
         * @brief Non-owning pointer to the Application instance the window belongs to.
         */
        helios::app::Application* application = nullptr;

        /**
         * @brief A non-owning pointer to the Window instance owning this struct.
         */
        helios::window::Window* window = nullptr;
    };

}
