module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>


module helios.ext.glfw.app.GLFWApplication;

import helios.window.Window;
import helios.ext.glfw.app.GLFWApplication;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.window.GLFWWindowConfig;

using namespace helios::window;
using namespace helios::ext::glfw::window;

namespace helios::ext::glfw::app {

    GLFWWindow& GLFWApplication::createWindow(const GLFWWindowConfig& cfg) {
        if (!window_) {
            window_ = std::make_unique<GLFWWindow>(cfg);
        } else {
            throw std::runtime_error("A window was already created for this app instance.");
        }

        if (const auto glfw_window  = dynamic_cast<GLFWWindow*>(window_.get())) {
            std::ignore = glfw_window->show();

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

        return dynamic_cast<GLFWWindow&>(*window_);
    }


    void GLFWApplication::setCurrent(Window& win) {

        if (const auto glfw_window  = dynamic_cast<GLFWWindow*>(&win)) {

            // 1. makes the context of the specified window current
            //    for the calling thread (@todo extract?)
            glfwMakeContextCurrent(glfw_window->nativeHandle());

            // 2. set the framebuffersize callback
            glfwSetFramebufferSizeCallback(
                glfw_window->nativeHandle(),
                glfw_window->frameBufferSizeCallback()
            );

            // 3. set the viewport for the rendering device
            const math::vec4& viewport = glfw_window->viewport();
            renderingDevice_->setViewport(viewport[0], viewport[1],
                viewport[2], viewport[3]
            );

        } else {
            throw std::runtime_error("Cannot init: Missing GLFWWindow.");
        }

        inputManager_->observe(win);
        current_ = &win;
    }


    [[nodiscard]] Window* GLFWApplication::current() const noexcept  {
        return current_;
    };


    GLFWWindow& GLFWApplication::createWindow(const WindowConfig& cfg) {
        if (window_ != nullptr) {
            return dynamic_cast<GLFWWindow&>(*window_);
        }

        auto const* tmp_cfg = dynamic_cast<GLFWWindowConfig const*>(&cfg);
        if (!tmp_cfg) {
            throw std::invalid_argument(
                "GLFWApplication requires GLFWWindowConfig"
            );
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