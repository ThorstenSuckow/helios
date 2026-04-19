/**
 * @file TimerState.ixx
 * @brief Enumeration of possible game timer states.
 */
module;



export module helios.gameplay.timing.types.TimerState;

export namespace helios::gameplay::timing::types {

    /**
     * @brief Represents the state of a GameTimer.
     */
    enum class TimerState {

        /**
         * @brief Timer state is undefined (initial value).
         */
        Undefined = 0,

        /**
         * @brief Timer is actively accumulating time.
         */
        Running = 1,

        /**
         * @brief Timer is paused; elapsed time is preserved.
         */
        Paused = 2,

        /**
         * @brief Timer is finished.
         */
        Finished = 3,

        /**
         * @brief Timer was cancelled.
         */
        Cancelled = 4
    };

}