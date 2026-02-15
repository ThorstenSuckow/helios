/**
 * @file WorldResetListener.ixx
 * @brief Listener that resets the game world on specific transitions.
 */
module;


export module helios.engine.mechanics.gamestate.listeners.WorldResetListener;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;
import helios.engine.mechanics.gamestate.GameStateTransitionListener;


import helios.engine.core.data;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::mechanics::gamestate::listeners {

    /**
     * @brief Resets the GameWorld on specific state transitions.
     *
     * Triggers a world reset when transitioning from Title to Running,
     * from Paused to Title, when restarting from Paused, or when
     * exiting Running to Title.
     */
    class WorldResetListener : public GameStateTransitionListener {

    public:

        /**
         * @copydoc GameStateTransitionListener::onGameStateExit
         */
        void onGameStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState from
        ) noexcept {
            // noop
        }

        /**
         * @copydoc GameStateTransitionListener::onGameStateEnter
         */
        void onGameStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState to
        ) noexcept {


        }

        /**
         * @copydoc GameStateTransitionListener::onGameStateTransition
         */
        void onGameStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameStateTransitionContext transitionCtx
        ) noexcept override {

            const auto from = transitionCtx.from();
            const auto to = transitionCtx.to();
            const auto transitionId = transitionCtx.transitionId();

            bool reset = (from == GameState::Title && to == GameState::Running)   ||
                         (from == GameState::Paused && to == GameState::Title)    ||
                         (from == GameState::Paused &&
                             transitionId == GameStateTransitionId::RestartRequested &&
                             to == GameState::Running) ||
                         (from == GameState::Running && to == GameState::Title) ;

            if (reset) {
                updateContext.gameWorld().reset();
            }

        }




    };

}

