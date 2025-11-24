/**
 * @file Application.ixx
 * @brief Entry point and central manager for helios applications.
 */
module;

#include <memory>
#include <vector>
#include <list>

export module helios.app.Application;

import helios.app.controller.Controller;
import helios.rendering.RenderingDevice;
import helios.rendering.RenderTarget;
import helios.input.InputManager;
import helios.event.EventManager;
import helios.window.Window;
import helios.window.WindowConfig;
import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::app::Application"
export namespace helios::app {

    /**
     * @brief Class providing the entry point for a helios application.
     *
     * `Application` serves as the central hub for the helios framework, offering convenient access
     * and management for key subsystems such as the EventManager, the InputManager and the
     * RenderingDevice.
     *
     * It also allows for the registration of `helios.app.controller.Controller`s,
     * which can provide specialized logic and management for the various subsystems and related events.
     */
    class Application {

    protected:
        /**
         * @brief Shared logger instance for all Application objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief Flag indicating whether the application has been initialized.
         */
        bool initialized_ = false;

        /**
         * @brief The rendering device owned by this application.
         */
        std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice_;

        /**
         * @brief The input manager owned by this application.
         */
        std::unique_ptr<input::InputManager> inputManager_;

        /**
         * @brief The event manager owned by this application.
         */
        std::unique_ptr<event::EventManager> eventManager_;

        /**
         * @brief A list containing all windows managed by this application.
         */
        std::list<std::unique_ptr<helios::window::Window>> windowList_;

        /**
         * @brief Collection of controllers registered with this application.
         */
        std::vector<std::unique_ptr<helios::app::controller::Controller>> controllers_;

    public:

        /**
         * @todo free resource allocations from renderingDevice,
         * window and InputManager
         */
        virtual ~Application() = default;


        /**
         * @brief Creates a new application instance.
         *
         * The application will take ownership of the specified constructor arguments.
         *
         * @param renderingDevice Unique rendering device owned by the application.
         *
         * @param inputManager Unique input manager instance.
         *
         * @param eventManager Unique event manager instance.
         */
        explicit Application(
            std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
            std::unique_ptr<helios::input::InputManager> inputManager,
            std::unique_ptr<helios::event::EventManager> eventManager
            );


        /**
         * @brief Registers a new controller with this application.
         *
         * Makes sure that the controller subscribes to the event manager's dispatcher
         * of this application. The application receives ownership of the specified controller.
         *
         * @param controller Unique pointer to the controller to add.
         */
        void addController(std::unique_ptr<helios::app::controller::Controller> controller) noexcept;

        /**
         * @brief Creates the container for the native window and performs all
         * necessary steps to properly initialize it.
         *
         * @param renderTarget Render target that wraps the framebuffer used by the window.
         *
         * @param cfg Window configuration defining properties such as title, dimensions, and hints.
         *
         * @return Reference to the created window instance.
         *
         * @throws std::invalid_argument If the configuration was invalid.
         */
        [[nodiscard]] virtual helios::window::Window& createWindow(std::unique_ptr<helios::rendering::RenderTarget> renderTarget, const helios::window::WindowConfig& cfg) = 0;

        /**
         * @brief Initializes the application.
         *
         * Performs any bootstrapping necessary for this application and provides an idiomatic
         * entry point for APIs using this application. This method ensures that all controllers
         * are initialized and subscribe to this application's event manager dispatcher.
         *
         * @throws std::runtime_error If the application was already initialized.
         */
        virtual void init();


        /**
         * @brief Sets the application's active window.
         *
         * Advises the input manager to poll this window's events.
         *
         * @param win Reference to the window that becomes the current active window.
         */
        virtual void setCurrent(helios::window::Window& win) = 0;


        /**
         * @brief Returns true if the specified window is owned by this application,
         * otherwise false.
         *
         * @param win The window to look up in this application's window list.
         *
         * @return True if the window is owned by this application, otherwise false.
         */
        [[nodiscard]] bool hasWindow(const helios::window::Window& win) const noexcept;


        /**
         * @brief Returns the currently focused window, or nullptr
         * if none exists or no window is being treated as current.
         *
         * @return Pointer to the current window, or nullptr if none is active.
         */
        [[nodiscard]] virtual helios::window::Window* current() const noexcept = 0;


        /**
         * @brief Returns the input manager owned by this application.
         *
         * @return Reference to the input manager instance.
         */
        [[nodiscard]] helios::input::InputManager& inputManager() const {
            return *inputManager_;
        };

        /**
         * @brief Returns the rendering device owned by this application.
         *
         * @return Reference to the rendering device instance.
         */
        [[nodiscard]] helios::rendering::RenderingDevice& renderingDevice() const {
            return *renderingDevice_;
        };

        /**
         * @brief Returns the event manager owned by this application.
         *
         * @return Reference to the event manager instance.
         */
        [[nodiscard]] helios::event::EventManager& eventManager() const {
            return *eventManager_;
        }


    };

}
