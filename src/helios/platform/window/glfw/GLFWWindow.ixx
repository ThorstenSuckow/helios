module;

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

export module helios.platform.window.glfw:GLFWWindow;

import helios.platform.window.core;
import :GLFWWindowConfig;

export namespace helios::platform::window::glfw {

    class GLFWWindow : public Window {

    private:
        GLFWwindow* nativeHandle_ = nullptr;
        const GLFWframebuffersizefun framebufferSizeCallback_;

    public:
        explicit GLFWWindow(const GLFWWindowConfig& cfg) :
            Window(cfg),
            framebufferSizeCallback_(cfg.frameBufferSizeCallback) {};

            /********************
             * Overrides
             *******************/
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

        [[nodiscard]] Window& setShouldClose(bool close) override {
            glfwSetWindowShouldClose(nativeHandle_, close);
            return dynamic_cast<GLFWWindow&>(*this);
        }

        [[nodiscard]] bool shouldClose() const override {
            if (nativeHandle_ == nullptr) {
                return true;
            }
            return glfwWindowShouldClose(nativeHandle_);
        }


        /********************
         * Specifics
         *******************/
        [[nodiscard]] GLFWframebuffersizefun framebufferSizeCallback() const noexcept  {
            return framebufferSizeCallback_;
        }

        [[nodiscard]] GLFWwindow* nativeHandle() const {
            return nativeHandle_;
        }

        void destroy() {
            glfwDestroyWindow(nativeHandle_);
        }
    };

}