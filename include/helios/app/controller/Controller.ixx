module;

export module helios.app.controller.Controller;

import helios.event.Dispatcher;

export namespace helios::app::controller {

    /**
     * Abstract base class for application controllers.
     *
     * Derived classes are expected to implement methods for event subscription/handling
     * and optional initialization.
     */
    class Controller {

    public:
        virtual ~Controller() = default;

        /**
         * Registers this controller with the specified Dispatcher.
         *
         * This method should be called whenever a controller is interested in abn event dispatched
         * by the specified Dispatcher.
         *
         * @param dispatcher
         */
        virtual void subscribeTo(helios::event::Dispatcher& dispatcher) = 0;

        /**
         * Initializes this controller.
         * The initialization state is defined by deriving classes.
         *
         * @return bool true if the controller was successfully initialized and is
         * in a valid state, otherwise false, signaling this controller should not be used further.
         */
        virtual bool init() = 0;
    };


}