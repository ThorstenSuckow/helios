/**
 * @file LambdaStateListener.ixx
 * @brief Lambda-based listener for state transitions.
 */
module;

#include <functional>
#include <cstdint>
#include <utility>

export module helios.state.listeners.LambdaStateListener;

import helios.state.StateTransitionListener;
import helios.runtime.world.UpdateContext;

import helios.state.types;

import helios.runtime.world.GameObject;

using namespace helios::state::types;
export namespace helios::state::listeners {

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
        using OnEnterCallback = std::function<void(helios::runtime::world::UpdateContext&, const StateType)>;

        /**
         * @brief Callback type for state exit events.
         */
        using OnExitCallback = std::function<void(helios::runtime::world::UpdateContext&, const StateType)>;

        /**
         * @brief Callback type for state transition events.
         */
        using OnTransitionCallback = std::function<void(helios::runtime::world::UpdateContext&, const StateTransitionContext<StateType>)>;

        /**
         * @brief Callback invoked when entering a state.
         */
        OnEnterCallback onEnterCallback_ = nullptr;

        /**
         * @brief Callback invoked during state transition.
         */
        OnTransitionCallback onTransitionCallback_= nullptr;

        /**
         * @brief Callback invoked when exiting a state.
         */
        OnExitCallback onExitCallback_= nullptr;


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
         * @brief Constructs a listener with only an enter callback.
         *
         * @param onEnterCallback Called when entering a state.
         */
        explicit LambdaStateListener(
           OnExitCallback onEnterCallback
         ) : onEnterCallback_(std::move(onEnterCallback)) {}

        /**
         * @brief Constructs a listener with only a transition callback.
         *
         * @param onTransitionCallback Called during the transition.
         */
        explicit LambdaStateListener(
            OnTransitionCallback onTransitionCallback
         ) : onTransitionCallback_(std::move(onTransitionCallback)) {}



        /**
         * @copydoc StateTransitionListener::onStateExit
         */
        void onStateExit(
            helios::runtime::world::UpdateContext& updateContext,
            const StateType from
        ) noexcept override {
            if (onExitCallback_) {
                onExitCallback_(updateContext, from);
            }
        }

        /**
         * @copydoc StateTransitionListener::onStateEnter
         */
        void onStateEnter(
            helios::runtime::world::UpdateContext& updateContext,
            const StateType to
        ) noexcept override {
            if (onEnterCallback_) {
                onEnterCallback_(updateContext, to);
            }
        }

        /**
         * @copydoc StateTransitionListener::onStateTransition
         */
        void onStateTransition(
            helios::runtime::world::UpdateContext& updateContext,
            const StateTransitionContext<StateType> transitionCtx
        ) noexcept override {
            if (onTransitionCallback_) {
                onTransitionCallback_(updateContext, transitionCtx);
            }
        };




    };

}

