/**
 * @file StateTransitionListener.ixx
 * @brief Interface for state transition observers.
 */
module;


export module helios.engine.state.StateTransitionListener;

import helios.engine.runtime.world.UpdateContext;

import helios.engine.state.types;


export namespace helios::engine::state {

    using namespace helios::engine::state::types;

    /**
     * @brief Interface for observing state transitions.
     *
     * @details Listeners are notified during state transitions in order:
     * 1. onStateExit() - before leaving the current state
     * 2. onStateTransition() - during the transition
     * 3. onStateEnter() - after entering the new state
     *
     * @tparam StateType The state enum type.
     *
     * @see StateManager
     * @see LambdaStateListener
     */
    template<typename StateType>
    class StateTransitionListener {


    public:
        virtual ~StateTransitionListener() = default;

        /**
         * @brief Called when exiting a state.
         *
         * @param updateContext The current frame's update context.
         * @param from The state being exited.
         */
        virtual void onStateExit(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const StateType from
        ) noexcept = 0;

        /**
         * @brief Called when entering a state.
         *
         * @param updateContext The current frame's update context.
         * @param to The state being entered.
         */
        virtual void onStateEnter(
           helios::engine::runtime::world::UpdateContext& updateContext,
           const StateType to
       ) noexcept = 0;

        /**
         * @brief Called during a state transition.
         *
         * @param updateContext The current frame's update context.
         * @param transitionCtx The complete transition context.
         */
        virtual void onStateTransition(
            helios::engine::runtime::world::UpdateContext& updateContext,
            const StateTransitionContext<StateType> transitionCtx
        ) noexcept = 0;




    };

}

