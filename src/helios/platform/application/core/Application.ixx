module;

#include <memory>
#include <vector>

export module helios.platform.application.core:Application;

import helios.platform.application.controller.core;

import helios.rendering.core;
import helios.platform.input;
import helios.platform.window.core;
import helios.event;


using namespace helios::platform::window::core;
using namespace helios::platform::application::controller::core;

export namespace helios::platform::application::core {

    class Application {

    protected:
        std::unique_ptr<rendering::core::RenderingDevice> renderingDevice_;
        std::unique_ptr<input::InputManager> inputManager_;
        std::unique_ptr<event::EventManager> eventManager_;
        std::unique_ptr<Window> window_;

        std::vector<std::unique_ptr<Controller>> controller_;

    public:

        explicit Application(
            std::unique_ptr<rendering::core::RenderingDevice> renderingDevice,
            std::unique_ptr<input::InputManager> inputManager,
            std::unique_ptr<event::EventManager> eventManager
            ):
            renderingDevice_(std::move(renderingDevice)),
            inputManager_(std::move(inputManager)),
            eventManager_(std::move(eventManager))
        {
        };


        Application& addController(std::unique_ptr<Controller> controller);

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
        virtual Application& setCurrent(Window& win) = 0;

        /**
         * Returns the currently focused window, or nullptr
         * if none exists / no window is being treated as current()
         * @return
         */
        [[nodiscard]] virtual Window* current() const noexcept = 0;

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
         *Returns the EventManager owned by this application
         *
         */
        [[nodiscard]] event::EventManager& eventManager() const {
            return *eventManager_;
        }


    };

}
