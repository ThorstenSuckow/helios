module;

#include <memory>
#include <vector>

export module helios.app.Application;

import helios.app.controller.Controller;
import helios.rendering.RenderingDevice;
import helios.input.InputManager;
import helios.event.EventManager;
import helios.window.Window;
import helios.window.WindowConfig;
import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::app::Application"
export namespace helios::app {

    /**
     * Class providing the entry point for a helios application.
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
         * The const reference to the logger used with this Adapter.
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(HELIOS_LOG_SCOPE);

        bool initialized_ = false;

        std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice_;
        std::unique_ptr<input::InputManager> inputManager_;
        std::unique_ptr<event::EventManager> eventManager_;
        std::unique_ptr<helios::window::Window> window_;

        std::vector<std::unique_ptr<helios::app::controller::Controller>> controllers_;

    public:

        /**
         * @todo free resource allocations from renderingDevice,
         * window and InputManager
         */
        virtual ~Application() = default;


        /**
         * Creates a new Application instance.
         * The Application will take ownership of the specified constructor arguments.
         *
         * @param renderingDevice
         * @param inputManager
         * @param eventManager
         */
        explicit Application(
            std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
            std::unique_ptr<helios::input::InputManager> inputManager,
            std::unique_ptr<helios::event::EventManager> eventManager
            );


        /**
         * Registers a new controller with this application.
         * Makes sure that the controller subscribes to the EventManager's Dispatcher
         * of this Application.
         * The Application receives ownership of the specified controller.
         *
         * @note This implementation assumes that controllers are added before
         * initializing the app. If controllers are added after the Application was
         * initialized, the current implementation will throw a runtime exception.
         * Future-iterations should consider whether this constraint should be softened.
         *
         * @param controller a unique_ptr to the controller to add.
         *
         * @throws std::runtime_error if the Application was already initialized.
         */
        void addController(std::unique_ptr<helios::app::controller::Controller> controller);

        /**
         * Creates the container for the native window and performs all
         * necessary steps to properly initialize it.
         *
         * @param WindowConfig
         *
         * @return Window
         *
         * @throws std::invalid_argument if the configuration was invalid
         */
        [[nodiscard]] virtual helios::window::Window& createWindow(const helios::window::WindowConfig& cfg) = 0;

        /**
         * Inits the Application. Any bootstrapping necessary for this application
         * should be done here and provide an idiomatic entry point for any
         * API using this Application.
         * This method makes sure that the controllers are initialized and subscribe
         * to this Application's EventManager's dispatcher.
         *
         * @return void
         *
         * @throws std::runtime_error if the Application was already initialized.
         */
        virtual void init();


        /**
         * Set's the application's active window. Advises the
         * InputManager to poll this window's events.
         *
         * @param win
         *
         * @return Window
         */
        virtual void setCurrent(helios::window::Window& win) = 0;


        /**
         * Returns the currently focused window, or nullptr
         * if none exists / no window is being treated as current()
         *
         * @return Window
         */
        [[nodiscard]] virtual helios::window::Window* current() const noexcept = 0;


        /**
         * Returns the InputManager owned by this Application.
         *
         * @return InputManager
         */
        [[nodiscard]] helios::input::InputManager& inputManager() const {
            return *inputManager_;
        };

        /**
         * Returns the InputManager owned by this Application.
         *
         * @return RenderingDevice
         */
        [[nodiscard]] helios::rendering::RenderingDevice& renderingDevice() const {
            return *renderingDevice_;
        };

        /**
         * Returns the EventManager owned by this application
         *
         * @return EventManager
         */
        [[nodiscard]] helios::event::EventManager& eventManager() const {
            return *eventManager_;
        }


    };

}
