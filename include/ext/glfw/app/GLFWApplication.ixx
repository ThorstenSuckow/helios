/**
 * @file GLFWApplication.ixx
 * @brief GLFW-specific Application implementation.
 */
module;

#include <memory>
#include <vector>


export module helios.ext.glfw.app.GLFWApplication;

import helios.app.Application;
import helios.window.Window;
import helios.window.WindowConfig;
import helios.input.InputManager;
import helios.rendering.RenderingDevice;
import helios.event.EventManager;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.window.GLFWWindowConfig;
import helios.rendering.RenderTarget;

import helios.ext.glfw.app.GLFWRAIIGuard;

export namespace helios::ext::glfw::app {

    /**
     * @brief An Application implementation for glfw-based environments.
     *
     * This application owns a `GLFWRAIIGuard`.
     */
    class GLFWApplication : public helios::app::Application {

    private:
        /**
         * @brief A pointer to the currently active window of the Application, or `nullptr`
         * if there is no currently active window.
         */
        helios::window::Window* current_ = nullptr;

        /**
         * @brief RAII Guard for glfw initialization and termination.
         */
        GLFWRAIIGuard glfwRaiiGuard_;

    public:
        /**
         * @brief Constructs a new `GLFWApplication` instance.
         *
         * @param renderingDevice Unique rendering device owned by the application.
         * @param inputManager Unique input manager instance.
         * @param eventManager Unique event manager instance.
         */
        explicit GLFWApplication(
            std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
            std::unique_ptr<helios::input::InputManager> inputManager,
            std::unique_ptr<helios::event::EventManager> eventManager);


        /**
         * @copydoc helios::app::Application::init()
         */
        void init() override;


        /**
         * @brief Creates a new GLFWWindow and add it to this Application's windows collection.
         * The Application takes ownership of the window.
         *
         * Delegates to Window::show() for showing the window and initializes the associated rendering
         * device if necessary, e.g. for glfw an additional `glfwMakeContextCurrent()` is called
         * so the rendering device can be safely initialized, then immediately passes a
         * `nullptr` to the `glfwMakeContextCurrent()` to force calling APIs to explicitly
         * call setCurrent() on the constructed window.
         * The render target size is synced to the current framebuffer dimensions reported by
         * `glfwGetFramebufferSize()`.
         *
         * @return Reference to the created window instance.
         *
         * @see https://www.glfw.org/docs/latest/group__window.html#ga0e2637a4161afb283f5300c7f94785c9
         * @return A ref to the window created.
         */
        helios::ext::glfw::window::GLFWWindow& createWindow(
            std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
            const helios::ext::glfw::window::GLFWWindowConfig& cfg
        );


        /**
         * @copydoc helios::app::Application::createWindow()
         */
        helios::ext::glfw::window::GLFWWindow& createWindow(
            std::unique_ptr<helios::rendering::RenderTarget> renderTarget,
            const helios::window::WindowConfig& cfg
        ) override;


        /**
         * @brief Sets the current window for this application.
         *
         * Ensures `glfwMakeContextCurrent()` is called to bind the correct context for
         * subsequent glfw operations and re-registers the framebuffer size callback on
         * the targeted window.
         *
         * @param win Reference to the window that becomes the current context owner.
         *
         * @throws std::invalid_argument If `win` is not a `GLFWWindow` or not owned by this application.
         */
        void setCurrent(helios::window::Window& win) override;

        /**
         * @copydoc helios::app::Application::current()
         */
        [[nodiscard]] helios::window::Window* current() const noexcept override;

    };


}