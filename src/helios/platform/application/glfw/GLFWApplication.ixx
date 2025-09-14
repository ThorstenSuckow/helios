module;

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
            Application(renderingDevice, std::move(inputManager))
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        GLFWWindow& createWindow(const GLFWWindowConfig& cfg) {
            if (!window_) {
                window_ = std::make_unique<GLFWWindow>(cfg);
            } else {
                throw std::runtime_error("A window was already created for this app instance.");
            }

            return dynamic_cast<GLFWWindow&>(*window_);
        }

        GLFWApplication& focus(Window& win) override {
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

            if (const auto glfw_window  = dynamic_cast<GLFWWindow*>(window_.get())) {
                std::ignore = glfw_window->show();
                glfwMakeContextCurrent(glfw_window->nativeHandle());
                glfwSetFramebufferSizeCallback(
                    glfw_window->nativeHandle(),
                    glfw_window->framebufferSizeCallback()
                );
            } else {
                throw std::runtime_error("Cannot init: Missing GLFWWindow.");
            }

            focus(*window_);
            renderingDevice_->init();

            return dynamic_cast<GLFWApplication&>(*this);
        }

    };


}