module;

#include <glad/gl.h>
#include <glfw/glfw3.h>
#include <memory>
#include <stdexcept>
#include <iostream>

module helios.ext.glfw.app.GLFWApplication;

import helios.window.Window;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.window.GLFWWindowConfig;

using namespace helios::window;
using namespace helios::ext::glfw::window;

namespace helios::ext::glfw::app {


    GLFWApplication::GLFWApplication(
               std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
               std::unique_ptr<helios::input::InputManager> inputManager,
               std::unique_ptr<helios::event::EventManager> eventManager):
               Application(
                   std::move(renderingDevice),
                   std::move(inputManager),
                   std::move(eventManager)
                   ),
                    glfwRaiiGuard_(){}


    GLFWWindow& GLFWApplication::createWindow(const GLFWWindowConfig& cfg) {

        auto window = std::make_unique<GLFWWindow>(cfg);

        if (const auto glfw_window  = window.get()) {
            if (!glfw_window->show()) {
                const std::string msg = "Cannot show window.";
                logger_.error(msg);
                throw std::runtime_error(msg);
            }

            // the first window inits the rendering device in this case, since glad requires
            // context created by glfw window
            if (!renderingDevice_->initialized()) {
                glfwMakeContextCurrent(glfw_window->nativeHandle());
                renderingDevice_->init();
                // we intentionally set the previous context to NULL
                // to enforce call to setCurrent()
                glfwMakeContextCurrent(nullptr);
            }
        } else {
            throw std::runtime_error("Cannot create: Missing GLFWWindow.");
        }

        windowList_.emplace_back(std::move(window));

        return *static_cast<GLFWWindow*>(windowList_.back().get());
    }


    void GLFWApplication::setCurrent(Window& win) {

        if (!hasWindow(win)) {
            const std::string msg = "Window is not owned by this Application.";
            logger_.error(msg);
            throw std::runtime_error(msg);
        }

        if (const auto glfw_window  = dynamic_cast<GLFWWindow*>(&win)) {

            logger_.info(std::format("Setting Window {0} as current", win.guid().value()));
            // 1. makes the context of the specified window current
            //    for the calling thread (@todo extract?)
            glfwMakeContextCurrent(glfw_window->nativeHandle());

            // 2. set the framebuffersize callback
            glfwSetFramebufferSizeCallback(
                glfw_window->nativeHandle(),
                glfw_window->frameBufferSizeCallback()
            );

            // 3. set the viewport for the rendering device
            const math::vec4i& viewport = glfw_window->viewport();
            renderingDevice_->setViewport(viewport[0], viewport[1],
                viewport[2], viewport[3]
            );

        } else {
            const std::string msg = "Cannot init: Missing GLFWWindow.";
            logger_.error(msg);
            throw std::runtime_error(msg);
        }

        inputManager_->observe(win);
        current_ = &win;
    }


    [[nodiscard]] Window* GLFWApplication::current() const noexcept  {
        return current_;
    };


    GLFWWindow& GLFWApplication::createWindow(const WindowConfig& cfg) {

        auto const* tmp_cfg = dynamic_cast<GLFWWindowConfig const*>(&cfg);
        if (!tmp_cfg) {
            std::string msg = "GLFWApplication requires GLFWWindowConfig";
            logger_.error(msg);
            throw std::invalid_argument(msg);
        }

        return createWindow(*tmp_cfg);
    }


    void GLFWApplication::init() {

        Application::init();

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    }

};