/**
 * @file PlayerSpawnListener.ixx
 * @brief Listener that activates the player on match state transition.
 */
module;

#include <charconv>
#include <cstdint>
#include <utility>

export module helios.engine.mechanics.match.listeners.PlayerSpawnListener;

import helios.engine.mechanics.match.MatchStateTransitionListener;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.match.types;

import helios.engine.ecs.GameObject;

export namespace helios::engine::mechanics::match::listeners {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::ecs;

    /**
     * @brief Activates a player GameObject when entering the PlayerSpawn state.
     *
     * This listener holds a reference to the player GameObject and activates it
     * when the match transitions to the PlayerSpawn state.
     */
    class PlayerSpawnListener : public MatchStateTransitionListener {

        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a listener for the specified player GameObject.
         *
         * @param go The player GameObject to activate on spawn.
         */
        explicit PlayerSpawnListener(GameObject go) : gameObject_{go} {}

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateExit
         */
        virtual void onMatchStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState from
        ) noexcept override {

        }

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateEnter
         */
        virtual void onMatchStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState to
        ) noexcept override {
            if (to == MatchState::PlayerSpawn) {
                gameObject_.setActive(true);
            }
            if (to == MatchState::PlayerDeath || to == MatchState::GameOver) {
                gameObject_.setActive(false);
            }

        }

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateTransition
         */
        virtual void onMatchStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchStateTransitionContext transitionCtx
        ) noexcept override {
        };




    };

}

