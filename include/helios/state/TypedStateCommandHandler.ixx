/**
 * @file TypedStateCommandHandler.ixx
 * @brief Typed interface for state command handling.
 */
module;

export module helios.state.TypedStateCommandHandler;

import helios.state.StateCommandHandler;
import helios.state.commands;


export namespace helios::state {

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
        const helios::state::commands::StateCommand<StateType>& gameStateCommand
        ) noexcept = 0;


    };

}