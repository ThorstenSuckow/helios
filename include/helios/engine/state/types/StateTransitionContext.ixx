/**
 * @file StateTransitionContext.ixx
 * @brief Context information for state transitions.
 */
module;


export module helios.engine.state.types.StateTransitionContext;

import helios.engine.state.types.StateTransitionId;
import helios.engine.state.types.StateTransitionType;

export namespace helios::engine::state::types {

    /**
     * @brief Captures the complete context of a state transition.
     *
     * @details Contains the source state, target state, and the transition
     * ID that triggered the change. Passed to listeners during state changes.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionListener
     * @see StateManager
     */
    template<typename StateType>
    class StateTransitionContext {

        using StateTransitionIdType = typename StateTransitionId<StateType>::Type;

        /**
         * @brief The state being transitioned from.
         */
        StateType from_;

        /**
         * @brief The transition that triggered this change.
         */
        StateTransitionIdType transitionId_;

        /**
         * @brief The state being transitioned to.
         */
        StateType to_;

    public:

        /**
         * @brief Constructs a transition context.
         *
         * @param from The source state.
         * @param to The target state.
         * @param transitionId The transition identifier.
         */
        explicit StateTransitionContext(
            const StateType from,
            const StateType to,
            const StateTransitionIdType transitionId
        ) : from_(from), to_(to), transitionId_(transitionId) {}

        /**
         * @brief Returns the target state.
         *
         * @return The state being transitioned to.
         */
        [[nodiscard]] StateType to() const noexcept {
            return to_;
        }

        /**
         * @brief Returns the source state.
         *
         * @return The state being transitioned from.
         */
        [[nodiscard]] StateType from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the transition identifier.
         *
         * @return The ID of the transition.
         */
        [[nodiscard]] StateTransitionIdType transitionId() const noexcept {
            return transitionId_;
        }

    };

}