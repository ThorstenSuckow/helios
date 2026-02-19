/**
 * @file StateCommandHandler.ixx
 * @brief Base class for state command handlers.
 */
module;

export module helios.engine.state.StateCommandHandler;

import helios.engine.state.commands;


export namespace helios::engine::state {

    /**
     * @brief Abstract base for type-erased state command handling.
     *
     * @details Provides a common interface for all state command handlers
     * regardless of state type. Used by GameWorld to store handlers
     * polymorphically.
     *
     * @see TypedStateCommandHandler
     * @see StateManager
     */
    class StateCommandHandler {

    public:

        virtual ~StateCommandHandler() = default;

    };

}