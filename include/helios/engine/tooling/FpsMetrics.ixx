/**
 * @file FpsMetrics.ixx
 * @brief Module defining the FpsMetrics class for frame rate analysis and monitoring.
 */
module;

#include <deque>

export module helios.engine.tooling.FpsMetrics;

import helios.engine.tooling.FrameStats;

export namespace helios::engine::tooling {

    /**
     * @class FpsMetrics
     * @brief Aggregates and analyzes frame timing data over a rolling window.
     *
     * FpsMetrics collects FrameStats data from multiple frames and calculates
     * average FPS and frame times. It maintains a configurable history buffer
     * for smoothing out momentary fluctuations in frame rate.
     *
     * This class is particularly useful for debugging, profiling, and displaying
     * performance metrics in development tools or debug overlays.
     *
     * @par Usage Example:
     * ```cpp
     * helios::engine::tooling::FpsMetrics metrics;
     * metrics.setHistorySize(120);
     *
     * // In game loop:
     * helios::engine::tooling::FrameStats stats = framePacer.sync();
     * metrics.addFrame(stats);
     *
     * float fps          = metrics.getFps();
     * float avgFrameTime = metrics.getFrameTimeMs();
     * ```
     */
    class FpsMetrics {

        /**
         * @brief Stores a rolling history of frame statistics.
         *
         * `history_` holds the full timing data (work time, wait time, total frame time)
         * for a specified number of recent frames. This buffer is used to compute
         * average frame rate and frame time metrics over a configurable window.
         *
         * The use of a `std::deque` allows efficient addition and removal
         * of frame timing records while maintaining the order of events.
         */
        std::deque<helios::engine::tooling::FrameStats> history_;

        /**
         * @brief Size of the history buffer for storing frame statistics.
         *
         * Determines the maximum number of frames for which the frame timing data
         * is retained to calculate performance metrics such as average FPS and
         * frame times. A larger history size smooths out short-term variations
         * but may reduce responsiveness to sudden changes in performance.
         *
         * This variable is initialized to a default value of 60 but can be adjusted
         * based on the application's requirements for performance analysis.
         */
        size_t historySize_ = 60;

        /**
         * @brief Stores the calculated average frames per second (FPS).
         *
         * The `avgFps_` variable holds the computed average FPS value based on the
         * accumulated frame timing data in `history_`. It is recomputed lazily when
         * one of the query functions (e.g. `getFps()`) is called after new frames
         * have been added.
         */
        float avgFps_ = 0.0f;

        /**
         * @brief Stores the average frame time in milliseconds.
         *
         * Represents the total frame time (including work and wait) averaged over
         * the frames currently stored in the history buffer. The value is stored
         * in milliseconds and recomputed lazily when queried via the public getters
         * after new frames have been added.
         */
        float avgFrameTime_ = 0.0f;

        /**
         * @brief Tracks the most recent frame's work time in milliseconds.
         *
         * Represents the duration of work performed during the last processed frame.
         * This value is derived from the most recent `FrameStats` in the history and
         * expressed in milliseconds. It is updated when the metrics are recomputed
         * (lazy evaluation triggered by the getters).
         */
        float lastWorkTime_ = 0.0f;

        /**
         * @brief Tracks the idle time in milliseconds for the most recent frame.
         *
         * Represents the duration the system waited (idle time) during the last
         * processed frame. This value is derived from the most recent `FrameStats`
         * in the history and expressed in milliseconds. It is updated when the
         * metrics are recomputed (lazy evaluation triggered by the getters).
         */
        float lastWaitTime_ = 0.0f;

        /**
         * @brief Tracks the total number of frames processed since initialization.
         *
         * The frameCount_ variable accumulates the count of frames that have been
         * added via `addFrame()`. It starts at 0 and increments with each call,
         * serving as a global counter independent of the current history size.
         */
        unsigned long long frameCount_ = 0;

        /**
         * @brief Indicates whether cached metrics need to be recomputed.
         *
         * Set to true whenever new frame data is added or the history changes in a
         * way that invalidates the cached values. The next call to one of the public
         * query functions will trigger a recomputation in `update()`.
         */
        bool needsUpdate_ = true;

        /**
         * @brief Recomputes cached metrics if needed.
         *
         * If `needsUpdate_` is true and the history is non-empty, this function
         * updates `lastWorkTime_`, `lastWaitTime_`, `avgFrameTime_` and `avgFps_`
         * based on the current contents of `history_` and then clears the flag.
         *
         * If the history is empty or the cache is already up to date, this function
         * returns immediately without modifying any values.
         *
         * @note This function is safe to call in hot paths and does not throw under
         * normal conditions; it is therefore marked `noexcept`.
         */
        void update() noexcept;

    public:

        /**
         * @brief Adds a frame's statistics to the metrics history.
         *
         * Processes the provided FrameStats, updates the rolling history,
         * and marks cached values as dirty so they will be recomputed on
         * the next query. Old frames are automatically removed when the
         * history buffer exceeds the configured size. Additionally, the
         * `frameCount_` member is increased by one for each call.
         *
         * @param stats Frame statistics to add to the history.
         *
         * @note When used together with FramePacer, a typical usage pattern is:
         * @code
         * helios::engine::tooling::FrameStats stats = framePacer.sync();
         * metrics.addFrame(stats);
         * @endcode
         * This should usually be called once per frame at the end of the
         * frame loop, after timing information for the current frame has
         * been measured.
         */
        void addFrame(const helios::engine::tooling::FrameStats& stats);

        /**
         * @brief Sets the size of the frame history buffer.
         *
         * Determines how many frames are used for calculating averages.
         * Larger values produce smoother metrics but respond slower to changes.
         *
         * @param size Number of frames to keep in history (default: 60).
         *
         * @note Common values: 30 for roughly half-second average at 60 FPS,
         *       60 for roughly one-second average at 60 FPS.
         */
        void setHistorySize(size_t size);

        /**
         * @brief Gets the current history buffer size.
         *
         * @return Maximum number of frames kept in history.
         */
        [[nodiscard]] size_t getHistorySize() const noexcept;

        /**
         * @brief Gets the average frames per second.
         *
         * Triggers a lazy recomputation of the underlying metrics if
         * new frames have been added since the last query.
         *
         * @return Average FPS calculated over the frames stored in the history buffer.
         */
        [[nodiscard]] float getFps() noexcept;

        /**
         * @brief Gets the average frame time in milliseconds.
         *
         * Triggers a lazy recomputation of the underlying metrics if needed.
         *
         * @return Average total frame time (work + wait) in milliseconds.
         */
        [[nodiscard]] float getFrameTimeMs() noexcept;

        /**
         * @brief Gets the most recent frame's work time.
         *
         * Triggers a lazy recomputation if required.
         *
         * @return Work time of the last processed frame in milliseconds.
         */
        [[nodiscard]] float getWorkTimeMs() noexcept;

        /**
         * @brief Gets the most recent frame's idle time.
         *
         * Triggers a lazy recomputation if required.
         *
         * @return Wait/idle time of the last processed frame in milliseconds.
         */
        [[nodiscard]] float getIdleTimeMs() noexcept;

        /**
         * @brief Gets the frame count.
         *
         * @return The total number of frames that have been added via `addFrame()`.
         */
        [[nodiscard]] unsigned long long getFrameCount() const noexcept;

        /**
         * @brief Gets the average frame time in seconds.
         *
         * Convenience wrapper returning the same value as `getFrameTimeMs()`
         * converted from milliseconds to seconds.
         *
         * @return Average total frame time (work + wait) in seconds.
         */
        [[nodiscard]] float getFrameTimeSeconds() noexcept;

        /**
         * @brief Gets the most recent frame's work time in seconds.
         *
         * Convenience wrapper returning the same value as `getWorkTimeMs()`
         * converted from milliseconds to seconds.
         *
         * @return Work time of the last processed frame in seconds.
         */
        [[nodiscard]] float getWorkTimeSeconds() noexcept;

        /**
         * @brief Gets the most recent frame's idle time in seconds.
         *
         * Convenience wrapper returning the same value as `getIdleTimeMs()`
         * converted from milliseconds to seconds.
         *
         * @return Wait/idle time of the last processed frame in seconds.
         */
        [[nodiscard]] float getIdleTimeSeconds() noexcept;

        /**
         * @brief Gets the complete frame history.
         *
         * @return Const reference to the deque containing the full frame
         *         statistics history.
         *
         * @note Useful for rendering frame time graphs or diagnostic views
         *       in debug overlays.
         */
        [[nodiscard]] const std::deque<helios::engine::tooling::FrameStats>& getHistory() const noexcept;

        /**
         * @brief Clears all collected metrics and resets to initial state.
         *
         * Removes all stored frame statistics and resets cached metrics and
         * counters to their default values. After calling this function,
         * all query functions will report zeros until new frames are added.
         */
        void reset();
    };
}
