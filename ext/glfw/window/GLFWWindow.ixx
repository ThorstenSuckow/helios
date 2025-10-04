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
        bool show() noexcept override {
            if (nativeHandle_ != nullptr) {
                logger_.warn("Window already shown.");
                return true;
            }

            nativeHandle_ = glfwCreateWindow(
                width_, height_, title_.c_str(), nullptr, nullptr);

            if (nativeHandle_ == nullptr) {
                logger_.error("Failed to creat GLFW window");
                return false;
            }

            return true;
        }


        void swapBuffers() const noexcept override {
            glfwSwapBuffers(nativeHandle_);
        }


        void pollEvents() const noexcept override {
            glfwPollEvents();
        }


        ~GLFWWindow() override {
            destroy();
        }

        void setShouldClose(bool close) override {
            glfwSetWindowShouldClose(nativeHandle_, close);
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