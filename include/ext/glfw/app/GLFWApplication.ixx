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

import helios.ext.glfw.app.GLFWRAIIGuard;

export namespace helios::ext::glfw::app {

    /**
     * An Application implementation for glfw-based environments.
     *
     * This application owns a `GLFWRAIIGuard`.
     */
    class GLFWApplication : public helios::app::Application {

    private:
        /**
         * A pointer to the currently active window of the Application, or `nullptr`
         * if there is no currently active window.
         */
        helios::window::Window* current_ = nullptr;

        /**
         * RAII Guard for glfw initialization and termination.
         */
        GLFWRAIIGuard glfwRaiiGuard_;

    public:
        /**
         * Constructs a new `GLFWApplication` instance.
         *
         * @param renderingDevice
         * @param inputManager
         * @param eventManager
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
         * Creates a new GLFWWindow and add it to this Application's windows collection.
         * The Application takes ownership of the window.
         * Delegates to Window::show() for showing the window and initializes the associated rendering
         * device if necessary, e.g. for glfw an additional `glfwMakeContextCurrent()` is called
         * so the rendering device can be safely initialized, then immediately passes a
         * `nullptr` to the `glfwMakeContextCurrent()` to force calling APIs to explicitly
         * call setCurrent() on the constructed window.
         *
         * @param cfg The `GLFWWindowConfig` used for initializing the window.
         *
         * @return A ref to the window created.
         */
        helios::ext::glfw::window::GLFWWindow& createWindow(const helios::ext::glfw::window::GLFWWindowConfig& cfg);


        /**
         * @copydoc helios::app::Application::createWindow()
         */
        helios::ext::glfw::window::GLFWWindow& createWindow(const helios::window::WindowConfig& cfg) override;


        /**
         * Sets the current Window for this Application.
         * Makes sure that `glfwContextCurrent` is called for setting a
         * current context for glfw and subsequent glfw-operations.
         * It also re-registers `glfwSetFramebufferSizeCallback` with the
         * callback function of the Window.
         *
         * @param win A ref to the window to set as current.
         *
         * @throws if the specified Window is not a GLFWWindow or if the window is
         * not owned by this application.
         *
         * @todo the window should be signaled that it's now current window, for setting
         * the frameBufferSizeCallback and calling glfwMakeContextCurrent.
         */
        void setCurrent(helios::window::Window& win) override;

        /**
         * @copydoc helios::app::Application::current()
         */
        [[nodiscard]] helios::window::Window* current() const noexcept override;

    };


}