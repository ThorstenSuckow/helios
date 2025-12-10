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
     * @brief Factory class for creating pre-configured GLFW-specific application components.
     *
     * Eases setup of examples and default applications by reducing required boilerplate code.
     */
    class GLFWFactory {

    public:

        /**
         * @brief Creates a pre-configured GLFWApplication instance for OpenGL rendering.
         *
         * @param title The default title used with the application's main window.
         * @param width The width for the main window.
         * @param height The height for the main window.
         * @param aspectRatioNumer Aspect ratio numerator.
         * @param aspectRatioDenom Aspect ratio denominator.
         *
         * @return A unique_ptr to the newly created GLFWApplication.
         */
        static std::unique_ptr<GLFWApplication> makeOpenGLApp(
            std::string title,
            int width = 800, int height = 600,
            int aspectRatioNumer = 0, int aspectRatioDenom = 0
            );


        /**
         * @brief Creates a default `GLFWWindowConfig` with the specific title.
         *
         * This method also makes sure that a proper frameBufferSizeCallback is configured,
         * assuming this config is used with a Window created by an application.
         *
         * @param title The title for the window configuration.
         * @param width The width for the main window.
         * @param height The height for the main window.
         * @param aspectRatioNumer Aspect ratio numerator.
         * @param aspectRatioDenom Aspect ratio denominator.
         *
         * @return A `GLFWWindowConfig` object with default properties like height and width.
         */
        static helios::ext::glfw::window::GLFWWindowConfig makeWindowCfg(
        std::string title, int width = 800, int height = 600,
        int aspectRatioNumer = 0, int aspectRatioDenom = 0);
    };
}