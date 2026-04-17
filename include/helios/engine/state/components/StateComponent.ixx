/**
 * @file StateComponent.ixx
 * @brief Component storing current state for an entity.
 */
module;

export module helios.engine.state.components.StateComponent;

import helios.engine.state.types.StateTransitionId;
import helios.engine.state.types.StateTransitionContext;


export namespace helios::engine::state::components {

    /**
     * @brief Stores the current state and last transition for an entity.
     *
     * @details Typically attached to the session entity to track game-wide
     * state. Updated by the StateManager during transitions.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateManager
     * @see StateTransitionContext
     */
    template<typename StateType>
    class StateComponent  {

        using StateTransitionIdType = helios::engine::state::types::StateTransitionIdType<StateType>;

        /**
         * @brief The state that was transitioned from.
         */
        StateType from_ = StateType::Undefined;

        /**
         * @brief The last transition that occurred.
         */
        StateTransitionIdType transitionId_ = StateTransitionIdType::Undefined;

        /**
         * @brief The current state.
         */
        StateType state_ = StateType::Undefined;

    public:

        /**
         * @brief Returns the current state.
         *
         * @return The current state value.
         */
        StateType state() const noexcept {
            return state_;
        }

        /**
         * @brief Returns the source state of the last transition.
         *
         * @return The state that was transitioned from.
         */
        StateType from() const noexcept {
            return from_;
        }

        /**
         * @brief Updates state from a transition context.
         *
         * @param stateTransitionContext The completed transition context.
         */
        void setStateFromTransitionContext(const helios::engine::state::types::StateTransitionContext<StateType> stateTransitionContext) noexcept {
            from_         = stateTransitionContext.from();
            state_        = stateTransitionContext.to();
            transitionId_ = stateTransitionContext.transitionId();
        }

        /**
         * @brief Returns the last transition ID.
         *
         * @return The transition that led to the current state.
         */
        StateTransitionIdType transitionId()  const noexcept {
            return transitionId_;
        }

    };


}