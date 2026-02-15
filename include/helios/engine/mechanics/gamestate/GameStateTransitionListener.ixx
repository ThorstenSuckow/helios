/**
 * @file GameStateTransitionListener.ixx
 * @brief Listener interface for game state transitions.
 */
module;


export module helios.engine.mechanics.gamestate.GameStateTransitionListener;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;


export namespace helios::engine::mechanics::gamestate {

    using namespace helios::engine::mechanics::gamestate::types;

    /**
     * @brief Abstract listener for game state transition events.
     *
     * Implementations can react to state exits, entries, and transitions.
     * Listeners are notified by the GameStateManager when state changes occur.
     *
     * @see GameStateManager
     */
    class GameStateTransitionListener {


    public:
        virtual ~GameStateTransitionListener() = default;

        /**
         * @brief Called when exiting a game state.
         *
         * @param updateContext The current update context.
         * @param from The state being exited.
         */
        virtual void onGameStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState from
        ) noexcept = 0;

        /**
         * @brief Called when entering a game state.
         *
         * @param updateContext The current update context.
         * @param to The state being entered.
         */
        virtual void onGameStateEnter(
           helios::engine::runtime::world::UpdateContext& updateContext,
           const GameState to
       ) noexcept = 0;

        /**
         * @brief Called during a state transition.
         *
         * @param updateContext The current update context.
         * @param transitionCtx Context containing from, to, and transition ID.
         */
        virtual void onGameStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameStateTransitionContext transitionCtx
        ) noexcept = 0;




    };

}

