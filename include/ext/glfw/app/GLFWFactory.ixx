/**
 * @file GLFWFactory.ixx
 * @brief Factory functions for creating GLFW-based Application and Window instances.
 */
module;

#include <memory>
#include <string>

export module helios.ext.glfw.app.GLFWFactory;

import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindowConfig;

export namespace helios::ext::glfw::app {


    /**
     * @brief A factory class for creating pre-configured glfw-specific application components.
     *
     * This factory eases setup of examples and default application and reduces required
     * boilerplate code.
     */
    class GLFWFactory {

    public:

        /**
         * @brief Creates a pre-configured GLFWApplication instance for OpenGL rendering.
         *
         * @param title The default title used with the application's main window.
         *
         * @return A unique_ptr to the newly created GFWLApplication.
         */
        static std::unique_ptr<GLFWApplication> makeOpenGLApp(std::string title);


        /**
         * @brief Creates a default `GLFWWindowConfig` with the specific title.
         *
         * This method also makes sure that a proper frameBufferSizeCallback is configured,
         * assuming this config is used with a Window created by an application.
         *
         * @param title The title for the window configuration.
         *
         * @return A `GLFWWindowConfig` object with default properties like height and width.
         */
        static helios::ext::glfw::window::GLFWWindowConfig makeWindowCfg(std::string title);
    };
}