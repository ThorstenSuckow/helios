module;

#include <iostream>
#include <GLFW/glfw3.h>

module helios.ext.glfw.window.GLFWWindow;

import helios.window.Window;

import helios.ext.glfw.window.GLFWWindowUserPointer;
import helios.ext.glfw.window.GLFWWindowConfig;


namespace helios::ext::glfw::window {

    GLFWWindow::~GLFWWindow() {
        destroy();
    }

    GLFWWindow::GLFWWindow(
        std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
        const GLFWWindowConfig& cfg
    ) :
        Window(std::move(renderTarget), cfg),
        frameBufferSizeCallback_(cfg.frameBufferSizeCallback) {
    };


    bool GLFWWindow::show() noexcept  {
        if (nativeHandle_ != nullptr) {
            logger_.warn("Window already shown.");
            return true;
        }


        logger_.info("Calling glfwCreateWindow().");
        nativeHandle_ = glfwCreateWindow(
            width_, height_, title_.c_str(), nullptr, nullptr);

        if (nativeHandle_ == nullptr) {
            logger_.error("Failed to creat GLFW window");
            return false;
        }

        return true;
    }


    void GLFWWindow::swapBuffers() const noexcept {
        glfwSwapBuffers(nativeHandle_);
    }


    void GLFWWindow::pollEvents() const noexcept {
        glfwPollEvents();
    }


    void GLFWWindow::setShouldClose(bool close) {
        glfwSetWindowShouldClose(nativeHandle_, close);
    }


    [[nodiscard]] bool GLFWWindow::shouldClose() const {
        if (nativeHandle_ == nullptr) {
            return true;
        }
        return glfwWindowShouldClose(nativeHandle_);
    }


    [[nodiscard]] GLFWframebuffersizefun GLFWWindow::frameBufferSizeCallback() const noexcept  {
        return frameBufferSizeCallback_;
    }


    void GLFWWindow::setFrameBufferSizeCallback(GLFWframebuffersizefun framebufferSizeCallback) noexcept {
        frameBufferSizeCallback_ = framebufferSizeCallback;
    }


    [[nodiscard]] GLFWwindow* GLFWWindow::nativeHandle() const noexcept {
        return nativeHandle_;
    }


    void GLFWWindow::destroy() const {
        if (nativeHandle_) {
            glfwDestroyWindow(nativeHandle_);
        }
    }


    void GLFWWindow::setWindowUserPointer(std::unique_ptr<GLFWWindowUserPointer> windowUserPointer) noexcept {
        windowUserPointer_ = std::move(windowUserPointer);
        glfwSetWindowUserPointer(nativeHandle_, windowUserPointer_.get());
    }


    [[nodiscard]] const GLFWWindowUserPointer& GLFWWindow::windowUserPointer() const noexcept {
        return *windowUserPointer_;
    }
};