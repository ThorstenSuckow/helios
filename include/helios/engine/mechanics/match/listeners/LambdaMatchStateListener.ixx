/**
 * @file LambdaMatchStateListener.ixx
 * @brief Lambda-based listener for match state transitions.
 */
module;

#include <functional>
#include <cstdint>
#include <utility>

export module helios.engine.mechanics.match.listeners.LambdaMatchStateListener;

import helios.engine.mechanics.match.MatchStateTransitionListener;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.mechanics.match.types;

import helios.engine.ecs.GameObject;

export namespace helios::engine::mechanics::match::listeners {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::ecs;

    /**
     * @brief Lambda-based implementation of MatchStateTransitionListener.
     *
     * @details Allows registering callbacks for match state transitions without
     * creating a dedicated listener subclass. Useful for simple, inline reactions
     * to state changes.
     *
     * ## Usage
     *
     * ```cpp
     * auto listener = std::make_unique<LambdaMatchStateListener>(
     *     [](auto& ctx, MatchState from) {  },
     *     [](auto& ctx, auto transitionCtx) {  },
     *     [](auto& ctx, MatchState to) {  }
     * );
     * stateManager.addMatchStateListener(std::move(listener));
     * ```
     *
     * @see MatchStateTransitionListener
     * @see MatchStateManager
     */
    class LambdaMatchStateListener : public MatchStateTransitionListener {

        /**
         * @brief Callback type for state enter events.
         */
        using OnEnterCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const MatchState)>;

        /**
         * @brief Callback type for state exit events.
         */
        using OnExitCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const MatchState)>;

        /**
         * @brief Callback type for state transition events.
         */
        using OnTransitionCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const MatchStateTransitionContext)>;

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
         * @param onExitCallback Called when exiting a match state.
         * @param onTransitionCallback Called during state transition.
         * @param onEnterCallback Called when entering a match state.
         */
        explicit LambdaMatchStateListener(
            OnExitCallback onExitCallback,
            OnTransitionCallback onTransitionCallback,
            OnEnterCallback onEnterCallback
        ) : onExitCallback_(std::move(onExitCallback)),
            onTransitionCallback_(std::move(onTransitionCallback)),
            onEnterCallback_(std::move(onEnterCallback)) {}

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateExit
         */
        virtual void onMatchStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState from
        ) noexcept override {
            onExitCallback_(updateContext, from);
        }

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateEnter
         */
        virtual void onMatchStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchState to
        ) noexcept override {

            onEnterCallback_(updateContext, to);
        }

        /**
         * @copydoc MatchStateTransitionListener::onMatchStateTransition
         */
        virtual void onMatchStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const MatchStateTransitionContext transitionCtx
        ) noexcept override {
            onTransitionCallback_(updateContext, transitionCtx);
        };




    };

}

