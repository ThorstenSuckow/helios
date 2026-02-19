/**
 * @file LambdaStateListener.ixx
 * @brief Lambda-based listener for state transitions.
 */
module;

#include <functional>
#include <cstdint>
#include <utility>

export module helios.engine.state.listeners.LambdaStateListener;

import helios.engine.state.StateTransitionListener;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.state.types;

import helios.engine.ecs.GameObject;

export namespace helios::engine::state::listeners {

    using namespace helios::engine::state::types;
    using namespace helios::engine::ecs;

    /**
     * @brief Lambda-based implementation of StateTransitionListener.
     *
     * @details Allows registering callbacks for state transition events
     * without creating a dedicated listener class. Useful for simple
     * reactions to state changes.
     *
     * @tparam StateType The state enum type.
     *
     * @see StateTransitionListener
     * @see StateManager
     */
    template<typename StateType>
    class LambdaStateListener : public StateTransitionListener<StateType> {

        /**
         * @brief Callback type for state enter events.
         */
        using OnEnterCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const StateType)>;

        /**
         * @brief Callback type for state exit events.
         */
        using OnExitCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const StateType)>;

        /**
         * @brief Callback type for state transition events.
         */
        using OnTransitionCallback = std::function<void(helios::engine::runtime::world::UpdateContext&, const StateTransitionContext<StateType>)>;

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
         * @brief Constructs a lambda state listener.
         *
         * @param onExitCallback Called when exiting a state.
         * @param onTransitionCallback Called during the transition.
         * @param onEnterCallback Called when entering a state.
         */
        explicit LambdaStateListener(
            OnExitCallback onExitCallback,
            OnTransitionCallback onTransitionCallback,
            OnEnterCallback onEnterCallback
        ) : onExitCallback_(std::move(onExitCallback)),
            onTransitionCallback_(std::move(onTransitionCallback)),
            onEnterCallback_(std::move(onEnterCallback)) {}

        /**
         * @copydoc StateTransitionListener::onStateExit
         */
        void onStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const StateType from
        ) noexcept override {
            onExitCallback_(updateContext, from);
        }

        /**
         * @copydoc StateTransitionListener::onStateEnter
         */
        void onStateEnter(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const StateType to
        ) noexcept override {

            onEnterCallback_(updateContext, to);
        }

        /**
         * @copydoc StateTransitionListener::onStateTransition
         */
        void onStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const StateTransitionContext<StateType> transitionCtx
        ) noexcept override {
            onTransitionCallback_(updateContext, transitionCtx);
        };




    };

}

