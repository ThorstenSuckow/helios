module;

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

export module helios.ext.glfw.window.GLFWWindow;

import helios.window.Window;

import helios.ext.glfw.window.GLFWWindowUserPointer;
import helios.ext.glfw.window.GLFWWindowConfig;


export namespace helios::ext::glfw::window {

    class GLFWWindow : public helios::window::Window {

    private:
        GLFWwindow* nativeHandle_ = nullptr;
        GLFWframebuffersizefun frameBufferSizeCallback_ = [](GLFWwindow* win, int with, int height){};
        std::unique_ptr<GLFWWindowUserPointer> windowUserPointer_;

    public:
        explicit GLFWWindow(const GLFWWindowConfig& cfg) :
            Window(cfg),
            frameBufferSizeCallback_(cfg.frameBufferSizeCallback){};

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
        [[nodiscard]] GLFWframebuffersizefun frameBufferSizeCallback() const noexcept  {
            return frameBufferSizeCallback_;
        }

        void setFrameBufferSizeCallback(GLFWframebuffersizefun framebufferSizeCallback) noexcept {
            frameBufferSizeCallback_ = framebufferSizeCallback;
        }

        [[nodiscard]] GLFWwindow* nativeHandle() const {
            return nativeHandle_;
        }

        void destroy() {
            glfwDestroyWindow(nativeHandle_);
        }

        GLFWWindow& setWindowUserPointer(std::unique_ptr<GLFWWindowUserPointer> windowUserPointer) noexcept {
            windowUserPointer_ = std::move(windowUserPointer);
            glfwSetWindowUserPointer(nativeHandle_, windowUserPointer_.get());
            return *this;
        }

        [[nodiscard]] const GLFWWindowUserPointer& windowUserPointer() const noexcept {
            return *windowUserPointer_;
        }
    };

}