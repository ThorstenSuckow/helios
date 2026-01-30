/**
 * @file GLFWFactory.ixx
 * @brief Factory functions for creating GLFW-based Application and Window instances.
 */
module;

#include <GLFW/glfw3.h>
#include <format>
#include <memory>
#include <string>

export module helios.ext.glfw.app.GLFWFactory;

import helios.window;
import helios.input;
import helios.event;
import helios.app.controller.BasicWindowRenderingController;
import helios.util.Guid;
import helios.event.EventManager;

import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindowConfig;
import helios.ext.glfw.window.GLFWWindowUserPointer;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.input.GLFWInputAdapter;

import helios.rendering.model.Material;
import helios.rendering.RenderTarget;

import helios.ext.opengl.rendering;

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
        ) {
            auto openGLDevice = std::make_unique<helios::ext::opengl::rendering::OpenGLDevice>(
                std::make_unique<helios::ext::opengl::rendering::OpenGLGlyphTextRenderer>()
            );
            auto deadzoneStrategy = std::make_unique<helios::input::gamepad::RadialDeadzoneStrategy>();
            auto inputManager = std::make_unique<helios::input::InputManager>(
                std::make_unique<helios::ext::glfw::input::GLFWInputAdapter>(std::move(deadzoneStrategy))
            );
            auto eventManager = std::make_unique<helios::event::BasicEventManager>(
                std::make_unique<helios::event::DequeEventQueue>(),
                std::make_unique<helios::event::Dispatcher>()
            );

            std::unique_ptr<GLFWApplication> app = std::make_unique<GLFWApplication>(
                std::move(openGLDevice),
                std::move(inputManager),
                std::move(eventManager)
            );

            app->init();
            auto cfg = makeWindowCfg(
                std::move(title),
                width,
                height,
                aspectRatioNumer,
                aspectRatioDenom
            );
            auto renderTarget = std::make_unique<helios::rendering::RenderTarget>();

            helios::ext::glfw::window::GLFWWindow& win = app->createWindow(std::move(renderTarget), cfg);
            app->addController(std::make_unique<helios::app::controller::BasicWindowRenderingController>(win));

            // set the window user pointer so the frameBufferSizeCallback does not break
            win.setWindowUserPointer(std::make_unique<helios::ext::glfw::window::GLFWWindowUserPointer>(
                app.get(),
                &win
            ));

            app->setCurrent(win);

            return std::move(app);
        }


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
            int aspectRatioNumer = 0, int aspectRatioDenom = 0
        ) {
            auto cfg             = helios::ext::glfw::window::GLFWWindowConfig{};
            cfg.title            = std::move(title);
            cfg.width            = width;
            cfg.height           = height;
            cfg.aspectRatioNumer = aspectRatioNumer;
            cfg.aspectRatioDenom = aspectRatioDenom;

            cfg.frameBufferSizeCallback = [] (GLFWwindow* nativeWin, const int width, const int height) {
                static const auto evtGuid = helios::util::Guid::generate();

                if (const auto* ptr = static_cast<helios::ext::glfw::window::GLFWWindowUserPointer*>(glfwGetWindowUserPointer(nativeWin))) {
                    auto event = std::make_unique<helios::window::event::FrameBufferResizeEvent>(
                        ptr->window->guid(), width, height,
                        // we will use the window's guid as the tag
                        ptr->window->guid().value()
                    );
                    ptr->application->eventManager().post(
                        std::move(event),
                        helios::event::PostPolicy::LATEST_WINS
                    );
                }
            };

            return cfg;
        }
    };
}