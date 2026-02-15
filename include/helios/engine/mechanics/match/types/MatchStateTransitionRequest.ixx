/**
 * @file MatchStateTransitionRequest.ixx
 * @brief Request for a match state transition.
 */
module;


export module helios.engine.mechanics.match.types.MatchStateTransitionRequest;

import helios.engine.mechanics.match.types.MatchState;
import helios.engine.mechanics.match.types.MatchStateTransitionId;
import helios.engine.mechanics.match.types.MatchStateTransitionType;

export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Encapsulates a request to transition from a given state.
     *
     * Contains the source state and the desired transition identifier.
     * The target state is determined by matching rules in the MatchStateManager.
     */
    class MatchStateTransitionRequest {

        MatchState from_;

        MatchStateTransitionId transitionId_;


    public:

        /**
         * @brief Constructs a transition request.
         *
         * @param from The current state from which to transition.
         * @param transitionId The identifier of the requested transition.
         */
        explicit MatchStateTransitionRequest(
            const MatchState from,
            const MatchStateTransitionId transitionId
        ) : from_(from), transitionId_(transitionId) {}

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