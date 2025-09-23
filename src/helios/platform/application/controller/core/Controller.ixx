module;

export module helios.platform.application.controller.core:Controller;

import helios.event.core;

export namespace helios::platform::application::controller::core {

    class Controller {

    public:
        virtual Controller& subscribeTo(event::core::Dispatcher& dispatcher) = 0;

        virtual Controller& init();

        virtual ~Controller() = default;
    };


}