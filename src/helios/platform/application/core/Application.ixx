module;

#include <memory>

export module helios.platform.application.core:Application;

import helios.rendering.core;
import helios.platform.input;
import helios.platform.window.core;

using namespace helios::platform::window::core;

export namespace helios::platform::application::core {

    class Application {

    protected:
        std::unique_ptr<Window> window_;
        rendering::core::RenderingDevice* renderingDevice_;
        std::unique_ptr<input::InputManager> inputManager_;


    public:

        explicit Application(
            rendering::core::RenderingDevice* renderingDevice,
            std::unique_ptr<input::InputManager> inputManager
            ):
            renderingDevice_(renderingDevice),
            inputManager_(std::move(inputManager))
        {};

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
         * Inits the Application. Calls necessary operations on the rendering
         * device passed to this application and shows the window. Initializes
         * the InputManager.
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
         * Returns the InputManager owned by this Application.
         * @return
         */
        [[nodiscard]] input::InputManager& inputManager() const {
            return *inputManager_;
        };

    };

}
