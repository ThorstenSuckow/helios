/**
 * @file MatchStateComponent.ixx
 * @brief Component for storing match state on an entity.
 */
module;

export module helios.engine.mechanics.match.components.MatchStateComponent;

import helios.engine.mechanics.match.types;

export namespace helios::engine::mechanics::match::components {

    /**
     * @brief Stores the current match state associated with an entity.
     *
     * @details Typically attached to a session entity to track the current
     * state of the match (e.g., Warmup, Playing, PlayerDeath). The component
     * also stores the transition ID that caused the current state, enabling
     * systems to react to specific transitions.
     *
     * @see MatchState
     * @see MatchStateTransitionId
     * @see MatchStateManager
     */
    struct MatchStateComponent  {

        /**
         * @brief The current match state.
         */
        helios::engine::mechanics::match::types::MatchState matchState_;

        /**
         * @brief The transition ID that caused the current state.
         */
        helios::engine::mechanics::match::types::MatchStateTransitionId matchStateTransitionId_;

    public:

        /**
         * @brief Returns the current match state.
         *
         * @return The current MatchState value.
         */
        [[nodiscard]] helios::engine::mechanics::match::types::MatchState matchState() const noexcept {
            return matchState_;
        }

        /**
         * @brief Sets the match state from a transition context.
         *
         * @details Updates both the match state and the transition ID from the
         * provided context. The state is set to the transition's target state.
         *
         * @param matchStateTransitionContext The context containing the new state and transition ID.
         */
        void setMatchState(
            const helios::engine::mechanics::match::types::MatchStateTransitionContext matchStateTransitionContext) noexcept {
            matchStateTransitionId_ = matchStateTransitionContext.transitionId();
            matchState_ = matchStateTransitionContext.to();
        }

        /**
         * @brief Returns the transition ID that caused the current state.
         *
         * @return The MatchStateTransitionId of the last transition.
         */
        [[nodiscard]] helios::engine::mechanics::match::types::MatchStateTransitionId matchStateTransitionId() const noexcept {
            return matchStateTransitionId_;
        }

    };


}