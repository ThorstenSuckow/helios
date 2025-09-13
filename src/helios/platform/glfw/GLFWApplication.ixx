module;

#include <GLFW/glfw3.h>
#include <memory>
#include <stdexcept>

#include "../../../../.build/cmake_build_release/_deps/benchmark-src/src/arraysize.h"

export module helios.platform.glfw:GLFWApplication;

import helios.platform.core;
import helios.rendering.core;
import :GLFWWindow;
import :GLFWWindowConfig;


export namespace helios::platform::glfw {

    class GLFWApplication : public Application {

    public:

        explicit GLFWApplication(rendering::core::RenderingDevice* renderingDevice):
            Application(renderingDevice)
        {
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        GLFWWindow& createWindow(const GLFWWindowConfig& cfg) {
            if (!window_) {
                window_ = std::make_unique<GLFWWindow>(cfg);
            }
            return dynamic_cast<GLFWWindow&>(*window_);
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

            renderingDevice_->init();

            return dynamic_cast<GLFWApplication&>(*this);
        }

    };


}