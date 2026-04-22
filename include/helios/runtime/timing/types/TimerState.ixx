/**
 * @file TimerState.ixx
 * @brief Enumeration of possible game timer states.
 */
module;



export module helios.runtime.timing.types.TimerState;

export namespace helios::runtime::timing::types {

    /**
     * @brief Represents the state of a Timer.
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