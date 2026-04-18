/**
 * @file DelayedStateCommand.ixx
 * @brief Timer-deferred command for requesting state transitions.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.state.commands.DelayedStateCommand;

import helios.gameplay.timing.types.GameTimerId;

import helios.engine.state.Bindings;

import helios.engine.state.types.StateTransitionRequest;

using namespace helios::engine::state::types;
using namespace helios::gameplay::timing::types;


export namespace helios::engine::state::commands {

    /**
     * @brief Command that pairs a state transition request with a timer.
     *
     * @details Unlike StateCommand, which triggers a transition immediately
     * during the next flush, DelayedStateCommand associates the request with
     * a GameTimerId. The TypedCommandBuffer holds the command until the
     * referenced timer expires, then forwards it to the StateManager.
     *
     * When the StateManager receives a DelayedStateCommand via submit(),
     * it extracts the StateTransitionRequest and enqueues it as a regular
     * StateCommand — the timer ID is not retained by the manager.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateCommand
     * @see StateTransitionRequest
     * @see StateManager
     */
    template<typename StateType>
    class DelayedStateCommand {

        /**
         * @brief The transition request to process.
         */
        StateTransitionRequest<StateType> transitionRequest_;

        /**
         * @brief The timer that gates this command.
         */
        GameTimerId timerId_;

    public:

        /**
         * @brief Constructs a delayed state command.
         *
         * @param transitionRequest The transition to perform when the timer fires.
         * @param timerId Identifier of the timer that triggers this command.
         */
        explicit DelayedStateCommand(StateTransitionRequest<StateType> transitionRequest, GameTimerId timerId)
            : transitionRequest_(transitionRequest), timerId_(timerId) {}



        /**
         * @brief Returns the transition request.
         *
         * @return The encapsulated transition request.
         */
        [[nodiscard]] types::StateTransitionRequest<StateType> transitionRequest() const noexcept {
            return transitionRequest_;
        }

        /**
         * @brief Returns the associated timer identifier.
         *
         * @return The GameTimerId that gates this command.
         */
        [[nodiscard]] GameTimerId gameTimerId() const noexcept {
            return timerId_;
        }


    };


}