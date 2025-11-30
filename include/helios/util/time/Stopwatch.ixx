/**
 * @file Stopwatch.ixx
 * @brief  A high-resolution timer based on std::chrono::steady_clock.
 */

module;

#include <chrono>

export module helios.util.time.Stopwatch;

export namespace helios::util::time {

    /**
     * @brief A high-resolution timer based on std::chrono::steady_clock.
     *
     * Used to measure elapsed time intervals with the highest precision
     * provided by the OS (suitable for frame time measurements and profiling).
     */
    class Stopwatch {

    private:
        // Initialize to now so elapsed() is well-defined even if start() wasn't called explicitly.
        std::chrono::time_point<std::chrono::steady_clock> start_{std::chrono::steady_clock::now()};

    public:
        /**
         * @brief Constructs and starts the stopwatch.
         *
         * The stopwatch is initialized to the current time so queries are
         * well-defined immediately after construction.
         */
        Stopwatch() noexcept = default;

        /**
         * @brief Starts (or restarts) the timer.
         * Captures the current timestamp.
         */
        void start() noexcept;

        /**
         * @brief Returns the time elapsed since start() was called.
         *
         * @return The elapsed time in milliseconds as a floating point value.
         */
        [[nodiscard]] float elapsedMs() const noexcept;

        /**
         * @brief Returns the time elapsed in seconds.
         *
         * @return The elapsed time in seconds as a floating point value.
         */
        [[nodiscard]] float elapsedSeconds() const noexcept;

    };
}