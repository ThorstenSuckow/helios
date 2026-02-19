/**
 * @file StateTransitionRequest.ixx
 * @brief Request to trigger a state transition.
 */
module;


export module helios.engine.state.types.StateTransitionRequest;


import helios.engine.state.types.StateTransitionId;
import helios.engine.state.types.StateTransitionType;

export namespace helios::engine::state::types {

    /**
     * @brief Represents a request to perform a state transition.
     *
     * @details Contains the current state and the desired transition ID.
     * The StateManager uses registered rules to determine the target state.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateCommand
     * @see StateManager
     */
    template<typename StateType>
    class StateTransitionRequest {

        using StateTransitionIdType = StateTransitionIdType<StateType>;

        /**
         * @brief The current state to transition from.
         */
        StateType from_;

        /**
         * @brief The transition to perform.
         */
        StateTransitionIdType transitionId_;


    public:

        /**
         * @brief Constructs a transition request.
         *
         * @param from The current state.
         * @param transitionId The transition to perform.
         */
        explicit StateTransitionRequest(
            const StateType from,
            const StateTransitionIdType transitionId
        ) : from_(from), transitionId_(transitionId) {}

        /**
         * @brief Returns the source state.
         *
         * @return The state to transition from.
         */
        [[nodiscard]] StateType from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the transition identifier.
         *
         * @return The ID of the requested transition.
         */
        [[nodiscard]] StateTransitionIdType transitionId() const noexcept {
            return transitionId_;
        }

    };

}