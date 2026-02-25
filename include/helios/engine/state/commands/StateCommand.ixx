/**
 * @file StateCommand.ixx
 * @brief Command for requesting state transitions.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.state.commands.StateCommand;


import helios.engine.state.Bindings;

import helios.engine.state.types;

using namespace helios::engine::state::types;

export namespace helios::engine::state::commands {

    /**
     * @brief Command that requests a state transition.
     *
     * @details Encapsulates a StateTransitionRequest and routes it through
     * the command dispatcher to the appropriate StateManager. The command
     * does not execute directly; instead it uses the visitor pattern via
     * accept() to be dispatched.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionRequest
     * @see StateCommandDispatcher
     * @see StateManager
     */
    template<typename StateType>
    class StateCommand {

        /**
         * @brief The transition request to process.
         */
        StateTransitionRequest<StateType> transitionRequest_;


    public:

        /**
         * @brief Constructs a state command.
         *
         * @param transitionRequest The transition request.
         */
        explicit StateCommand(StateTransitionRequest<StateType> transitionRequest)
            : transitionRequest_(transitionRequest) {}

        /**
         * @brief Returns the transition request.
         *
         * @return The encapsulated transition request.
         */
        [[nodiscard]] types::StateTransitionRequest<StateType> transitionRequest() const noexcept {
            return transitionRequest_;
        }


    };


}