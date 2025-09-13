module;

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

export module helios.platform.glfw:GLFWWindow;

import helios.platform.core;
import :GLFWWindowConfig;
import helios.util.Guid;

export namespace helios::platform::glfw {

    class GLFWWindow : public Window {

    private:
        GLFWwindow* nativeHandle_ = nullptr;
        const util::Guid guid_;
        const GLFWframebuffersizefun framebufferSizeCallback_;

    public:
        explicit GLFWWindow(const GLFWWindowConfig& cfg) :
            Window(cfg),
            guid_(util::Guid::generate()),
            framebufferSizeCallback_(cfg.frame_buffer_size_callback) {};

        [[nodiscard]] bool shouldClose() const override {
            if (nativeHandle_ == nullptr) {
                return true;
            }
            return glfwWindowShouldClose(nativeHandle_);
        }

        [[nodiscard]] GLFWWindow& show() override {
            if (nativeHandle_ != nullptr) {
                throw std::runtime_error("Window already shown.");
            }

            nativeHandle_ = glfwCreateWindow(
                width_, height_, title_.c_str(), nullptr, nullptr);

            if (nativeHandle_ == nullptr) {
                throw std::runtime_error("Failed to creat GLFW window");
            }

            return (*this);
        }


        [[nodiscard]] GLFWframebuffersizefun framebufferSizeCallback() const noexcept  {
            return framebufferSizeCallback_;
        }

        [[nodiscard]] GLFWwindow* nativeHandle() const {
            return nativeHandle_;
        }

        GLFWWindow& swapBuffers() override {
            glfwSwapBuffers(nativeHandle_);
            return *this;
        }

        GLFWWindow& pollEvents() override {
            glfwPollEvents();
            return *this;
        }


        ~GLFWWindow() override {
            destroy();
        }

        void destroy() {
            glfwDestroyWindow(nativeHandle_);
        }
    };

}