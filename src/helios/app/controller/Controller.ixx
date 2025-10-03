module;

export module helios.app.controller.Controller;

import helios.event.Dispatcher;

export namespace helios::app::controller {

    class Controller {

    public:
        virtual Controller& subscribeTo(helios::event::Dispatcher& dispatcher) = 0;

        virtual Controller& init();

        virtual ~Controller() = default;
    };


}