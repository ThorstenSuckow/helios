module;

#include <memory>


export module helios.platform.application.core:Application;

import helios.rendering.core;
import helios.platform.input;
import helios.platform.window.core;
import helios.event.core;


using namespace helios::platform::window::core;

export namespace helios::platform::application::core {

    class Application {

    protected:
        std::unique_ptr<event::core::EventQueue> eventQueue_;
        std::unique_ptr<Window> window_;
        std::unique_ptr<rendering::core::RenderingDevice> renderingDevice_;
        std::unique_ptr<input::InputManager> inputManager_;


    public:

        explicit Application(
            std::unique_ptr<rendering::core::RenderingDevice> renderingDevice,
            std::unique_ptr<input::InputManager> inputManager,
            std::unique_ptr<event::core::EventQueue> eventQueue
            ):
            renderingDevice_(std::move(renderingDevice)),
            inputManager_(std::move(inputManager)),
            eventQueue_(std::move(eventQueue)) {
        };

        /**
         * @todo free resource allocations from renderingDevice,
         * window and InputManager
         */
        virtual ~Application() = default;

        /**
         * Creates the container for the native window and performs all
         * necessary steps to properly initialize it.
         *
         * @param cfg
         * @return
         *
         * @throws std::invalid_argument if the configuration was invalid
         */
        virtual Window& createWindow(const WindowConfig& cfg) = 0;

        /**
         * Inits the Application. Any bootstrapping necessary for this application
         * should be done here and provide an idiomatic entry point for any
         * API using this Application.
         *
         * @return
         *
         * @throws std::runtime_error
         */
        virtual Application& init() = 0;

        /**
         * Set's the application's active window. Advises the
         * InputManager to poll this window's events.
         *
         * @param win
         * @return
         */
        virtual Application& focus(Window& win) = 0;

        /**
         * Returns the currently focused window, or nullptr
         * if none exists / no window is being treated as focused()
         * @return
         */
        [[nodiscard]] virtual Window* focused() const noexcept = 0;

        /**
         * Returns the InputManager owned by this Application.
         * @return
         */
        [[nodiscard]] input::InputManager& inputManager() const {
            return *inputManager_;
        };

        /**
         * Returns the InputManager owned by this Application.
         * @return
         */
        [[nodiscard]] rendering::core::RenderingDevice& renderingDevice() const {
            return *renderingDevice_;
        };

        /**
         *Returns the EventQueue owned by this application
         *
         */
        [[nodiscard]] event::core::EventQueue& eventQueue() const {
            return *eventQueue_;
        }

    };

}
