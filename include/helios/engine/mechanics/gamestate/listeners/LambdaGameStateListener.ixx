/**
 * @file LambdaGameStateListener.ixx
 * @brief Lambda-based listener for game state transitions.
 */
module;

#include <functional>
#include <cstdint>
#include <utility>

export module helios.engine.mechanics.gamestate.listeners.LambdaGameStateListener;

import helios.engine.mechanics.gamestate.GameStateTransitionListener;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.gamestate.types;

import helios.engine.ecs.GameObject;

export namespace helios::engine::mechanics::gamestate::listeners {

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::ecs;

    /**
     * @brief Lambda-based implementation of GameStateTransitionListener.
     *
     * @details Allows registering callbacks for game state transitions without
     * creating a dedicated listener subclass. Useful for simple, inline reactions
     * to state changes.
     *
     * ## Usage
     *
     * ```cpp
     * auto listener = std::make_unique<LambdaGameStateListener>(
     *     [](auto& ctx, GameState from) {  },
     *     [](auto& ctx, auto transitionCtx) {  },
     *     [](auto& ctx, GameState to) {  }
     * );
     * stateManager.addGameStateListener(std::move(listener));
     * ```
     *
     * @see GameStateTransitionListener
     * @see GameStateManager
     */
    class LambdaGameStateListener : public GameStateTransitionListener {

        /**
         * @brief Callback type for state enter events.
         */
        using OnEnterCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const GameState)>;

        /**
         * @brief Callback type for state exit events.
         */
        using OnExitCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const GameState)>;

        /**
         * @brief Callback type for state transition events.
         */
        using OnTransitionCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const GameStateTransitionContext)>;

        /**
         * @brief Callback invoked when entering a state.
         */
        OnEnterCallback onEnterCallback_;

        /**
         * @brief Callback invoked during state transition.
         */
        OnTransitionCallback onTransitionCallback_;

        /**
         * @brief Callback invoked when exiting a state.
         */
        OnExitCallback onExitCallback_;


    public:

        /**
         * @brief Constructs a listener with the specified callbacks.
         *
         * @param onExitCallback Called when exiting a game state.
         * @param onTransitionCallback Called during state transition.
         * @param onEnterCallback Called when entering a game state.
         */
        explicit LambdaGameStateListener(
            OnExitCallback onExitCallback,
            OnTransitionCallback onTransitionCallback,
            OnEnterCallback onEnterCallback
        ) : onExitCallback_(std::move(onExitCallback)),
            onTransitionCallback_(std::move(onTransitionCallback)),
            onEnterCallback_(std::move(onEnterCallback)) {}

        /**
         * @copydoc GameStateTransitionListener::onGameStateExit
         */
        virtual void onGameStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState from
        ) noexcept override {
            onExitCallback_(updateContext, from);
        }

        /**
         * @copydoc GameStateTransitionListener::onGameStateEnter
         */
        virtual void onGameStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameState to
        ) noexcept override {

            onEnterCallback_(updateContext, to);
        }

        /**
         * @copydoc GameStateTransitionListener::onGameStateTransition
         */
        virtual void onGameStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const GameStateTransitionContext transitionCtx
        ) noexcept override {
            onTransitionCallback_(updateContext, transitionCtx);
        };




    };

}

