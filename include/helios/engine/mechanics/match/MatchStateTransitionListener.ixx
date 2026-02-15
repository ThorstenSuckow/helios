/**
 * @file MatchStateTransitionListener.ixx
 * @brief Listener interface for match state transitions.
 */
module;

#include <cstdint>
#include <utility>

export module helios.engine.mechanics.match.MatchStateTransitionListener;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.match.types;

export namespace helios::engine::mechanics::match {

    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Abstract listener for match state transition events.
     *
     * Implementations can react to state exits, entries, and transitions.
     * Listeners are notified by the MatchStateManager when state changes occur.
     *
     * @see MatchStateManager
     */
    class MatchStateTransitionListener {


    public:
        virtual ~MatchStateTransitionListener() = default;

        /**
         * @brief Called when exiting a match state.
         *
         * @param updateContext The current update context.
         * @param from The state being exited.
         */
        virtual void onMatchStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState from
        ) noexcept = 0;

        /**
         * @brief Called when entering a match state.
         *
         * @param updateContext The current update context.
         * @param to The state being entered.
         */
        virtual void onMatchStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState to
        ) noexcept = 0;

        /**
         * @brief Called during a state transition.
         *
         * @param updateContext The current update context.
         * @param transitionCtx Context containing from, to, and transition ID.
         */
        virtual void onMatchStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchStateTransitionContext transitionCtx
        ) noexcept = 0;




    };

}

