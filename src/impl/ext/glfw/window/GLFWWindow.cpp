module;

#include <iostream>
#include <GLFW/glfw3.h>

module helios.ext.glfw.window.GLFWWindow;

import helios.window.Window;

import helios.ext.glfw.window.GLFWWindowUserPointer;
import helios.ext.glfw.window.GLFWWindowConfig;


namespace helios::ext::glfw::window {

    /**
     * Destructor. Delegates to #destroy()
     */
    GLFWWindow::~GLFWWindow() {
        destroy();
    }


    /**
     * Constructs a new GLFWWindow based in the provided configuration.
     * The underlying native window is not created until #show() is called.
     *
     * @param cfg A const ref to this window's configuration-
     */
    GLFWWindow::GLFWWindow(const GLFWWindowConfig& cfg) :
        Window(cfg),
        frameBufferSizeCallback_(cfg.frameBufferSizeCallback) {
    };

    /********************
     * Overrides
     *******************/

    /**
     * Creates and displays the underlying native `GlFWwindow`.
     *
     * This method initializes the native window. It does not set the current context,
     * which is actually done by GLFWApplication::setCurrent().
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga3555a418df92ad53f917597fe2f64aeb
     */
    bool GLFWWindow::show() noexcept  {
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


    /**
     * Swaps the front and back buffers of the window.
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
     */
    void GLFWWindow::swapBuffers() const noexcept {
        glfwSwapBuffers(nativeHandle_);
    }


    /**
     * Processes all events in the underlying Window's EventQueue.
     *
     * This method should be called regularly in the main loop.
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga37bd57223967b4211d60ca1a0bf3c832
     */
    void GLFWWindow::pollEvents() const noexcept {
        glfwPollEvents();
    }


    /**
     * @copydoc helios::window::Window::setShouldClose()
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
     */
    void GLFWWindow::setShouldClose(bool close) {
        glfwSetWindowShouldClose(nativeHandle_, close);
    }


    /**
     * @copydoc helios::window::Window::shouldClose()
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
     */
    [[nodiscard]] bool GLFWWindow::shouldClose() const {
        if (nativeHandle_ == nullptr) {
            return true;
        }
        return glfwWindowShouldClose(nativeHandle_);
    }


    /********************
     * Specifics
     *******************/
    /**
     * Returns the framebuffer resize callback function.
     *
     * @return the `GLFWframebuffersizefun` currently registered with this Window.
     */
    [[nodiscard]] GLFWframebuffersizefun GLFWWindow::frameBufferSizeCallback() const noexcept  {
        return frameBufferSizeCallback_;
    }


    /**
     * Sets the callback function for framebuffer resize events.
     *
     * @param framebufferSizeCallback The `GLFWframebuffersizefun` to set.
     */
    void GLFWWindow::setFrameBufferSizeCallback(GLFWframebuffersizefun framebufferSizeCallback) noexcept {
        frameBufferSizeCallback_ = framebufferSizeCallback;
    }


    /**
     * Returns the raw native GLFWwindow handle.
     *
     * This method should only be used in GLFW-related API parts, where underlying
     * glfw-functions require the native window handle.
     *
     * @return The `GLFWwindow*` handle, or a `nullptr` if not available.
     */
    [[nodiscard]] GLFWwindow* GLFWWindow::nativeHandle() const noexcept {
        return nativeHandle_;
    }


    /**
     * Removed the underlying native handle of the GLFWwindow.
     *
     * @see https://www.glfw.org/docs/latest/group__window.html#gacdf43e51376051d2c091662e9fe3d7b2
     */
    void GLFWWindow::destroy() const {
        if (nativeHandle_) {
            glfwDestroyWindow(nativeHandle_);
        }
    }


    /**
     * Sets the user-defined pointer for his GLFWWindow.
     *
     * This transfers ownership of the `GLFWWindowUserPointer`-object to this
     * Window and additionally associates its raw pointer with this Window's
     * native handle.
     *
     * @param windowUserPointer A unique_ptr to the `GLFWWindowUserPointer` object.
     * Ownership is transferred to this Window-instance.
     * @return
     */
    void GLFWWindow::setWindowUserPointer(std::unique_ptr<GLFWWindowUserPointer> windowUserPointer) noexcept {
        windowUserPointer_ = std::move(windowUserPointer);
        glfwSetWindowUserPointer(nativeHandle_, windowUserPointer_.get());
    }


    /**
     * Returns a const reference to the `GLFWWindowUserPointer` managed by this class.
     *
     * @return A const ref to this object's `GLFWWindowUserPointer`, or `nullptr` if
     * not available.
     */
    [[nodiscard]] const GLFWWindowUserPointer& GLFWWindow::windowUserPointer() const noexcept {
        return *windowUserPointer_;
    }
};