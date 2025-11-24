/**
 * @file GLFWWindow.ixx
 * @brief GLFW-based Window implementation.
 */
module;

#include <iostream>
#include <GLFW/glfw3.h>
#include <string>

export module helios.ext.glfw.window.GLFWWindow;

import helios.window.Window;

import helios.ext.glfw.window.GLFWWindowUserPointer;
import helios.ext.glfw.window.GLFWWindowConfig;
import helios.rendering.RenderTarget;

export namespace helios::ext::glfw::window {

    /**
     * @brief An OpenGL focused window implementation using GLFW.
     *
     * This class manages a `GLFWwindow` handle, providing methods for
     * creation, display, event polling and resource cleanup.
     *
     * @todo add glfwSetWindowRefreshCallback
     */
    class GLFWWindow : public helios::window::Window {

    private:
        /**
         * @brief The native GLFWwindow handle.
         */
        GLFWwindow* nativeHandle_ = nullptr;


        /**
         * @brief C-style callback function for framebuffer resize events.
         * This function pointer is passed to `glfwSetFramebufferSizeCallback`.
         */
        GLFWframebuffersizefun frameBufferSizeCallback_;

        /**
         * @brief A unique_ptr to the to an object holding user defined data for the native GLFWwindow.
         * This information gets forwarded using `glfwSetWindowUserPointer`
         */
        std::unique_ptr<GLFWWindowUserPointer> windowUserPointer_;

    public:
        /**
         * @brief Do not copy instances of GLFWWindow.
         */
        GLFWWindow(const GLFWWindow&) = delete;
        GLFWWindow& operator=(const GLFWWindow&) = delete;

        /**
         * @brief Destructor. Delegates to #destroy()
         */
        ~GLFWWindow() override;


        /**
         * @brief Constructs a new GLFWWindow based in the provided configuration.
         * The underlying native window is not created until #show() is called.
         *
         * @param cfg A const ref to this window's configuration-
         */
        explicit GLFWWindow(
            std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
            const GLFWWindowConfig& cfg
        );

        /********************
         * Overrides
         *******************/

        /**
         * @brief Creates and displays the underlying native `GlFWwindow`.
         *
         * This method initializes the native window. It does not set the current context,
         * which is actually done by GLFWApplication::setCurrent().
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga3555a418df92ad53f917597fe2f64aeb
         */
        bool show() noexcept override;


        /**
         * @brief Swaps the front and back buffers of the window.
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga15a5a1ee5b3c2ca6b15ca209a12efd14
         */
        void swapBuffers() const noexcept override;


        /**
         * @brief Processes all events in the underlying Window's EventQueue.
         *
         * This method should be called regularly in the main loop.
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga37bd57223967b4211d60ca1a0bf3c832
         */
        void pollEvents() const noexcept override;


        /**
         * @copydoc helios::window::Window::setShouldClose()
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga49c449dde2a6f87d996f4daaa09d6708
         */
        void setShouldClose(bool close) override;


        /**
         * @copydoc helios::window::Window::shouldClose()
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga24e02fbfefbb81fc45320989f8140ab5
         */
        [[nodiscard]] bool shouldClose() const override;


        /********************
         * Specifics
         *******************/
        /**
         * @brief Returns the framebuffer resize callback function.
         *
         * @return the `GLFWframebuffersizefun` currently registered with this Window.
         */
        [[nodiscard]] GLFWframebuffersizefun frameBufferSizeCallback() const noexcept;


        /**
         * @brief Sets the callback function for framebuffer resize events.
         *
         * Implementing APIs should consider this method as "package protected": It's purpose is
         * to register a function that delegates the native event into the Application's
         * EventQueue.
         *
         * @param framebufferSizeCallback The `GLFWframebuffersizefun` to set.
         */
        void setFrameBufferSizeCallback(GLFWframebuffersizefun framebufferSizeCallback) noexcept;


        /**
         * @brief Returns the raw native GLFWwindow handle.
         *
         * This method should only be used in GLFW-related API parts, where underlying
         * glfw-functions require the native window handle.
         *
         * @return The `GLFWwindow*` handle, or a `nullptr` if not available.
         */
        [[nodiscard]] GLFWwindow* nativeHandle() const noexcept;


        /**
         * @brief Removed the underlying native handle of the GLFWwindow.
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#gacdf43e51376051d2c091662e9fe3d7b2
         */
        void destroy() const;


        /**
         * @brief Sets the user-defined pointer for his GLFWWindow.
         *
         * This transfers ownership of the `GLFWWindowUserPointer`-object to this
         * Window and additionally associates its raw pointer with this Window's
         * native handle.
         *
         * @param windowUserPointer A unique_ptr to the `GLFWWindowUserPointer` object.
         * Ownership is transferred to this Window-instance.
         * @return
         */
        void setWindowUserPointer(std::unique_ptr<GLFWWindowUserPointer> windowUserPointer) noexcept;


        /**
         * @brief Returns a const reference to the `GLFWWindowUserPointer` managed by this class.
         *
         * @return A const reference to this object's `GLFWWindowUserPointer`.
         */
        [[nodiscard]] const GLFWWindowUserPointer& windowUserPointer() const noexcept;
    };

} // namespace helios::ext::glfw::window
