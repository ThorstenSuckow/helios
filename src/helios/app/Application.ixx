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

export namespace helios::app {

    class Application {

    protected:
        std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice_;
        std::unique_ptr<input::InputManager> inputManager_;
        std::unique_ptr<event::EventManager> eventManager_;
        std::unique_ptr<helios::window::Window> window_;

        std::vector<std::unique_ptr<helios::app::controller::Controller>> controller_;

    public:

        explicit Application(
            std::unique_ptr<rendering::RenderingDevice> renderingDevice,
            std::unique_ptr<input::InputManager> inputManager,
            std::unique_ptr<event::EventManager> eventManager
            );


        Application& addController(std::unique_ptr<helios::app::controller::Controller> controller);

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
        virtual helios::window::Window& createWindow(const helios::window::WindowConfig& cfg) = 0;

        /**
         * Inits the Application. Any bootstrapping necessary for this application
         * should be done here and provide an idiomatic entry point for any
         * API using this Application.
         *
         * @return
         *
         * @throws std::runtime_error
         */
        virtual Application& init() {

            for (auto& ctrl: controller_) {
                ctrl->init();
            }
            return *this;
        };

        /**
         * Set's the application's active window. Advises the
         * InputManager to poll this window's events.
         *
         * @param win
         * @return
         */
        virtual Application& setCurrent(helios::window::Window& win) = 0;

        /**
         * Returns the currently focused window, or nullptr
         * if none exists / no window is being treated as current()
         * @return
         */
        [[nodiscard]] virtual helios::window::Window* current() const noexcept = 0;

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
        [[nodiscard]] rendering::RenderingDevice& renderingDevice() const {
            return *renderingDevice_;
        };

        /**
         *Returns the EventManager owned by this application
         *
         */
        [[nodiscard]] event::EventManager& eventManager() const {
            return *eventManager_;
        }


    };

}
