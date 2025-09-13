module;

#include <memory>

export module helios.platform.core:Application;

import helios.rendering.core;
import :Window;


export namespace helios::platform {

    class Application {

    protected:
        std::unique_ptr<Window> window_;
        rendering::core::RenderingDevice* renderingDevice_;

    public:
        explicit Application(rendering::core::RenderingDevice* renderingDevice):
            renderingDevice_(renderingDevice)
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
         * device passed to this application and shows the window
         *
         * @return
         *
         * @throws std::runtime_error
         */
        virtual Application& init() = 0;
    };

}
