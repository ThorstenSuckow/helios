module;

#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>

export module helios.glfw.application:GLFWFactory;

import :GLFWApplication;

import helios.platform.input;
import helios.glfw.input;
import helios.glfw.window;
import helios.rendering.core;
import helios.rendering.opengl;
import helios.event.core;
import helios.platform.window.core.event;

export namespace helios::glfw::application {


    class GLFWFactory {

    public:

        static std::unique_ptr<GLFWApplication> makeOpenGLApp(std::string title) {

            auto openGLDevice = std::make_unique<rendering::opengl::OpenGLDevice>();
            auto inputManager = std::make_unique<InputManager>(
                std::make_unique<input::GLFWInput>()
                );
            auto eventManager = std::make_unique<event::core::EventManager>(
            std::make_unique<event::core::EventQueue>(),
            std::make_unique<event::core::Dispatcher>()
            );

            std::unique_ptr<GLFWApplication> app = std::make_unique<GLFWApplication>(
                std::move(openGLDevice),
                std::move(inputManager),
                std::move(eventManager)
            );

            app->init();
            auto cfg = makeWindowCfg(std::move(title));

            GLFWWindow& win = app->createWindow(cfg);

            // set the window user pointer so the frameBufferSizeCallback does not break
            win.setWindowUserPointer(std::make_unique<GLFWWindowUserPointer>(
                app.get(),
                &win
            ));

            //app.renderingDevice()->subscribe<platform::window::core::event::FrameBufferResizeEvent>(dispatcher);

            app->eventManager().subscribe<platform::window::core::event::FrameBufferResizeEvent>(
                [app = app.get(), w = &win] (const platform::window::core::event::FrameBufferResizeEvent& e) {
                if (app->current()->guid().value() == w->guid().value() && w->guid().value() == e.sourceGuid.value()) {
                    app->renderingDevice().setViewport(0, 0, e.width, e.height);
                }
            });

            app->setCurrent(win);

            return app;
        }


        static GLFWWindowConfig makeWindowCfg(std::string title) {
            auto cfg = GLFWWindowConfig{};
            cfg.title = std::move(title);

            cfg.frameBufferSizeCallback = [] (GLFWwindow* nativeWin, const int width, const int height) {
                if (const auto* ptr = static_cast<GLFWWindowUserPointer*>(glfwGetWindowUserPointer(nativeWin))) {
                    auto event = std::make_unique<platform::window::core::event::FrameBufferResizeEvent>(
                        ptr->window->guid(), width, height
                    );
                    ptr->application->eventManager().post(std::move(event));
                }
            };

            return cfg;
        }

    };


}