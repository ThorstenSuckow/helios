/**
* @file FrameStats.ixx
 * @brief Module defining the FrameStats structure for frame timing information.
 */

export module helios.engine.tooling.FrameStats;

export namespace helios::engine::tooling {
    /**
     * @struct FrameStats
     * @brief Contains timing statistics for a single frame.
     *
     * This structure holds detailed timing information about a frame's execution,
     * including work time (actual CPU processing) and wait time (idle time from
     * frame pacing).
     *
     * @note All time values are specified in **seconds**.
     */
    struct FrameStats {
        /**
         * @brief Total frame time in seconds.
         *
         * The sum of workTime and waitTime. Represents the complete duration
         * of the frame from start to synchronization. This value corresponds
         * to the `deltaTime` used for game logic.
         */
        float totalFrameTime = 0.0f; /**< Total frame time in seconds. */

        /**
         * @brief CPU processing time in seconds.
         *
         * The actual time spent executing frame logic, rendering,
         * and other computational tasks before synchronization.
         */
        float workTime = 0.0f; /**< CPU processing time in seconds. */

        /**
         * @brief Idle time in seconds.
         *
         * The time spent sleeping/waiting by the FramePacer to maintain
         * the target frame rate. This is 0.0f if no frame pacing is active
         * or if the frame took longer than the target time.
         */
        float waitTime = 0.0f; /**< Idle time in seconds. */

    };
}