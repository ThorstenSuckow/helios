module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>


export module helios.platform.application.glfw:GLFWApplication;

import helios.platform.application.core;
import helios.rendering.core;
import helios.platform.window.glfw;
import helios.platform.window.core;
import helios.platform.input.core;
import helios.platform.input;


using namespace helios::platform::application::core;
using namespace helios::platform::window::glfw;
using namespace helios::platform::window::core;


export namespace helios::platform::application::glfw {

    class GLFWApplication : public Application {


    public:

        explicit GLFWApplication(
            rendering::core::RenderingDevice* renderingDevice,
            std::unique_ptr<input::InputManager> inputManager):
            Application(renderingDevice, std::move(inputManager)) {

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
                    // to enforce call to focus()
                    glfwMakeContextCurrent(nullptr);
                }
            } else {
                throw std::runtime_error("Cannot create: Missing GLFWWindow.");
            }

            return dynamic_cast<GLFWWindow&>(*window_);
        }

        GLFWApplication& focus(Window& win) override {

            if (const auto glfw_window  = dynamic_cast<GLFWWindow*>(&win)) {

                // 1. makes the context of the specified window current
                //    for the calling thread (@todo extract?)
                glfwMakeContextCurrent(glfw_window->nativeHandle());

                // 2. set the framebuffersize callback
                glfwSetFramebufferSizeCallback(
                    glfw_window->nativeHandle(),
                    glfw_window->framebufferSizeCallback()
                );

                // 3. set the viewport for the rendering device
                // we will leave this code here for now, knowing that the design is
                // somewhat brittle: The window itself should know about the viewport,
                // not the application
                renderingDevice_->setViewport(
                    0, 0, glfw_window->width(), glfw_window->height());

            } else {
                throw std::runtime_error("Cannot init: Missing GLFWWindow.");
            }

            inputManager_->observe(win);

            return dynamic_cast<GLFWApplication&>(*this);
        }


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