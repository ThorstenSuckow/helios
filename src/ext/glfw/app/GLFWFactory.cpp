module;

#include <GLFW/glfw3.h>
#include <format>
#include <memory>
#include <string>

module helios.ext.glfw.app.GLFWFactory;

import helios.window.Window;
import helios.window.event.FrameBufferResizeEvent;
import helios.input.InputManager;
import helios.event.BasicEventManager;
import helios.event.DequeEventQueue;
import helios.event.Dispatcher;
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


import helios.ext.glfw.window.GLFWWindowConfig;
import helios.ext.opengl.rendering.OpenGLDevice;

using namespace helios::app::controller;
using namespace helios::event;
using namespace helios::input;
using namespace helios::window;
using namespace helios::window::event;
using namespace helios::util;

using namespace helios::ext::glfw::window;
using namespace helios::ext::glfw::input;

using namespace helios::ext::opengl::rendering;
using namespace helios::rendering;

namespace helios::ext::glfw::app {

    std::unique_ptr<GLFWApplication> GLFWFactory::makeOpenGLApp(
        std::string title, const int width, const int height,  const int aspectRatioNumer, const int aspectRatioDenom
    ) {

        auto openGLDevice = std::make_unique<OpenGLDevice>();
        auto inputManager = std::make_unique<InputManager>(
            std::make_unique<GLFWInputAdapter>()
            );
        auto eventManager = std::make_unique<BasicEventManager>(
        std::make_unique<DequeEventQueue>(),
        std::make_unique<Dispatcher>()
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
        auto renderTarget = std::make_unique<RenderTarget>();

        GLFWWindow& win = app->createWindow(std::move(renderTarget), cfg);
        app->addController(std::make_unique<BasicWindowRenderingController>(win));

        // set the window user pointer so the frameBufferSizeCallback does not break
        win.setWindowUserPointer(std::make_unique<GLFWWindowUserPointer>(
            app.get(),
            &win
        ));


        app->setCurrent(win);

        return app;
    }


    GLFWWindowConfig GLFWFactory::makeWindowCfg(
        std::string title, const int width, const int height, const int aspectRatioNumer, const int aspectRatioDenom
    ) {
        auto cfg             = GLFWWindowConfig{};
        cfg.title            = std::move(title);
        cfg.width            = width;
        cfg.height           = height;
        cfg.aspectRatioNumer = aspectRatioNumer;
        cfg.aspectRatioDenom = aspectRatioDenom;

        cfg.frameBufferSizeCallback = [] (GLFWwindow* nativeWin, const int width, const int height) {
            static const auto evtGuid = Guid::generate();

            if (const auto* ptr = static_cast<GLFWWindowUserPointer*>(glfwGetWindowUserPointer(nativeWin))) {
                auto event = std::make_unique<FrameBufferResizeEvent>(
                    ptr->window->guid(), width, height,
                    // we will use the window's guid as the tag
                    ptr->window->guid().value()
                );
                ptr->application->eventManager().post(
                    std::move(event),
                    PostPolicy::LATEST_WINS
                );
            }
        };

        return cfg;
    }

};
