/**
 * @file MatchStateTransitionContext.ixx
 * @brief Context information for a match state transition.
 */
module;


export module helios.engine.mechanics.match.types.MatchStateTransitionContext;

import helios.engine.mechanics.match.types.MatchState;
import helios.engine.mechanics.match.types.MatchStateTransitionId;
import helios.engine.mechanics.match.types.MatchStateTransitionType;

export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Encapsulates the context of a state transition.
     *
     * Provides information about the source state, target state,
     * and the transition identifier.
     */
    class MatchStateTransitionContext {

        MatchState from_;
        MatchStateTransitionId transitionId_;
        MatchState to_;

    public:

        /**
         * @brief Constructs a transition context.
         *
         * @param from The state being exited.
         * @param to The state being entered.
         * @param transitionId The identifier of the transition.
         */
        explicit MatchStateTransitionContext(
            const MatchState from ,
            const MatchState to,
            const MatchStateTransitionId transitionId
        ) : from_(from), to_(to), transitionId_(transitionId) {}

        /**
         * @brief Returns the target state.
         */
        [[nodiscard]] MatchState to() const noexcept {
            return to_;
        }

        /**
         * @brief Returns the source state.
         */
        [[nodiscard]] MatchState from() const noexcept {
            return from_;
        }

        /**
         * @brief Returns the transition identifier.
         */
        [[nodiscard]] MatchStateTransitionId transitionId() const noexcept {
            return transitionId_;
        }

    };

}