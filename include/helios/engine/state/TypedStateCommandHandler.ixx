/**
 * @file TypedStateCommandHandler.ixx
 * @brief Typed interface for state command handling.
 */
module;

export module helios.engine.state.TypedStateCommandHandler;

import helios.engine.state.StateCommandHandler;
import helios.engine.state.commands;


export namespace helios::engine::state {

    /**
     * @brief Typed interface for handling state commands.
     *
     * @details Extends StateCommandHandler with type-safe submit method
     * for a specific state type. StateManager implements this interface.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateCommandHandler
     * @see StateManager
     * @see StateCommand
     */
    template<typename StateType>
    class TypedStateCommandHandler : public StateCommandHandler {

    public:

        /**
         * @brief Submits a state command for processing.
         *
         * @param gameStateCommand The command to submit.
         *
         * @return True if the command was accepted.
         */
        virtual bool submit(
        const helios::engine::state::commands::StateCommand<StateType>& gameStateCommand
        ) noexcept = 0;


    };

}