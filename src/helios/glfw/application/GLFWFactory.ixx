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
            auto eventQueue = std::make_unique<event::core::EventQueue>();

            std::unique_ptr<GLFWApplication> app = std::make_unique<GLFWApplication>(
                std::move(openGLDevice),
                std::move(inputManager),
                std::move(eventQueue)
            );

            app->init();
            auto cfg = makeWindowCfg(std::move(title));

            GLFWWindow& win = app->createWindow(cfg);

            // set the window user pointer so the frameBufferSizeCallback does not break
            win.setWindowUserPointer(std::make_unique<GLFWWindowUserPointer>(
                app.get(),
                &win
            ));

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
                    ptr->application->eventQueue().push(std::move(event));
                    //ptr->window->addEvent(PendingEvent(FRAME_BUFFER_RESIZE, ));
                   /* std::cout << width << " " << height;
                    const math::vec4& viewport = ptr->window->viewport();
                    ptr->renderingDevice->setViewport(
                        static_cast<int>(viewport[0]),
                        static_cast<int>(viewport[1]),
                        width, height
                    )*/;
                }
            };

            return cfg;
        }

    };


}