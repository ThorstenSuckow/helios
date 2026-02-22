/**
 * @file TimerControlContext.ixx
 * @brief Context information for state transitions.
 */
module;


export module helios.engine.mechanics.timing.types.TimerControlContext;

import helios.engine.mechanics.timing.types.TimerState;
import helios.engine.core.data;

export namespace helios::engine::mechanics::timing::types {

    using namespace helios::engine::core::data;

    /**
     * @brief Context describing a requested timer state transition.
     *
     * Carries the target TimerState and the GameTimerId of the timer
     * to be controlled.
     *
     * @see TimerControlCommand
     * @see TimerManager
     */
    struct TimerControlContext {

        /**
         * @brief The target state for the timer.
         */
        const TimerState timerState = TimerState::Stopped;

        /**
         * @brief The id of the timer to control.
         */
        const GameTimerId gameTimerId;
    };

}