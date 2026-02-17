/**
 * @file DefaultMatchStateTransitionGuards.ixx
 * @brief Default guard functions for match state transitions.
 */
module;

export module helios.engine.mechanics.match.rules.guards.DefaultMatchStateTransitionGuards;

import helios.engine.mechanics.match.types.MatchStateTransitionRequest;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.Session;

export namespace helios::engine::mechanics::match::rules::guards {

    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Provides default guard functions for match state transitions.
     *
     * @details Guard functions are predicates that determine whether a state
     * transition is allowed. They are evaluated before a transition occurs and
     * can block it by returning false.
     *
     * All guards are static functions that take an UpdateContext and a
     * MatchStateTransitionRequest, returning true if the transition is allowed.
     *
     * @see MatchStateTransitionRequest
     * @see MatchStateManager
     */
    class DefaultMatchStateTransitionGuards {

    public:

        /**
         * @brief Guard that allows transition only if the player is active.
         *
         * @param updateContext The current frame's update context.
         * @param transitionRequest The requested state transition.
         *
         * @return True if the player entity exists and is active.
         */
        static bool isPlayerActive(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchStateTransitionRequest transitionRequest
        ) {
            auto playerOpt = updateContext.session().playerEntity();
            return playerOpt.has_value() && playerOpt->isActive();
        }

        /**
         * @brief Guard that allows transition only if the player is inactive.
         *
         * @param updateContext The current frame's update context.
         * @param transitionRequest The requested state transition.
         *
         * @return True if the player entity exists and is inactive.
         */
        static bool isPlayerInactive(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchStateTransitionRequest transitionRequest
        ) {
            auto playerOpt = updateContext.session().playerEntity();
            return playerOpt.has_value() && !playerOpt->isActive();
        }

    };


}
