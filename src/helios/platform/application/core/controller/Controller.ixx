module;

export module helios.platform.application.core.controller:Controller;

import helios.event.core;

export namespace helios::platform::application::core::controller {

    class Controller {

    public:
        virtual Controller& subscribeTo(event::core::Dispatcher& dispatcher) = 0;

        virtual Controller& init() {
            // intentionally left empty
            return *this;
        }

        virtual ~Controller() = default;
    };


}