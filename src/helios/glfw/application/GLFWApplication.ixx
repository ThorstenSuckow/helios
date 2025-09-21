module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>


export module helios.glfw.application:GLFWApplication;

import helios.platform.application.core;
import helios.rendering.core;
import helios.glfw.window;
import helios.platform.window.core;
import helios.platform.input.core;
import helios.platform.input;
import helios.event.core;

using namespace helios::platform::application::core;
using namespace helios::glfw::window;
using namespace helios::platform::window::core;
using namespace helios::platform::input;

export namespace helios::glfw::application {

    class GLFWApplication : public Application {

    private:
        Window* current_ = nullptr;

    public:

        explicit GLFWApplication(
            std::unique_ptr<rendering::core::RenderingDevice> renderingDevice,
            std::unique_ptr<InputManager> inputManager,
            std::unique_ptr<event::core::EventManager> eventManager):
            Application(
                std::move(renderingDevice),
                std::move(inputManager),
                std::move(eventManager)
                ) {

        }

        GLFWWindow& createWindow(const GLFWWindowConfig& cfg) {
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

        GLFWApplication& setCurrent(Window& win) override {

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

            return dynamic_cast<GLFWApplication&>(*this);
        }


        [[nodiscard]] Window* current() const noexcept override  {
            return current_;
        };


        GLFWWindow& createWindow(const WindowConfig& cfg) override {
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


        GLFWApplication& init() override {

            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

            return dynamic_cast<GLFWApplication&>(*this);
        }

    };


}