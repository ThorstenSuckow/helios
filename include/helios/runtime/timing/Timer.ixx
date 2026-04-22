/**
 * @file Timer.ixx
 * @brief A game timer that tracks elapsed time and supports state transitions.
 */
module;

export module helios.runtime.timing.Timer;

import helios.runtime.timing.types.TimerId;

import helios.runtime.timing.types;

using namespace helios::runtime::timing::types;
using namespace helios::runtime::timing::types;

export namespace helios::runtime::timing {

    /**
     * @brief A game timer identified by a TimerId.
     *
     * Tracks elapsed time while in the Running state. Each update increments
     * a revision counter that observers can use to detect changes.
     *
     * @see TimerManager
     * @see TimerBindingComponent
     */
    class Timer {

        /**
         * @brief Monotonically increasing revision counter, incremented on each update.
         */
        TimerRevision timerRevision_{};

        /**
         * @brief Unique identifier for this timer.
         */
        TimerId timerId_;

        /**
         * @brief Accumulated elapsed time in seconds.
         */
        float elapsed_{};

        /**
         * @brief Current state of the timer.
         */
        TimerState timerState_{};

        /**
         * @brief Optional duration limit in seconds.
         */
        float duration_ = 0.0f;

    public:

        /**
         * @brief Constructs a Timer with the given identifier.
         *
         * @param timerId The unique identifier for this timer.
         */
        explicit Timer(const helios::runtime::timing::types::TimerId timerId)
            : timerId_{timerId} {}

        /**
         * @brief Returns the timer identifier.
         *
         * @return The TimerId assigned to this timer.
         */
        [[nodiscard]] helios::runtime::timing::types::TimerId timerId() const noexcept {
            return timerId_;
        }

        /**
         * @brief Resets the elapsed time to zero and sets timer state to Undefined.
         *
         * @param state The new TimerState the timer should be resetted to.
         */
        void reset(const TimerState state = TimerState::Undefined) noexcept {
            elapsed_ = 0.0f;
            setState(state);
        }

        /**
         * @brief Resets timer by resetting its internal counter and
         * setting its state to TimerState::Running.
         *
         * @see reset
         */
        void restart() noexcept {
            reset(TimerState::Running);
        }

        /**
         * @brief Cancels this timer by setting its state to TimerState::Cancelled.
         *
         * @see reset
         */
        void cancel() noexcept {
            reset(TimerState::Cancelled);
        }

        /**
         * @brief Sets the duration limit.
         *
         * @param duration Duration in seconds.
         */
        void setDuration(float duration) {
            duration_ = duration;
        }

        /**
         * @brief Returns the duration limit.
         *
         * @return The duration in seconds.
         */
        [[nodiscard]] float duration() const noexcept {
            return duration_;
        }

        /**
         * @brief Returns the accumulated elapsed time.
         *
         * @return Elapsed time in seconds.
         */
        [[nodiscard]] float elapsed() const noexcept {
            return elapsed_;
        }

        /**
         * @brief Returns the current timer state.
         *
         * @return The current TimerState.
         */
        [[nodiscard]] TimerState state() const noexcept {
            return timerState_;
        }

        /**
         * @brief Sets the timer state.
         *
         * @param state The new TimerState.
         */
        void setState(const TimerState state) noexcept {
            timerState_ = state;
            timerRevision_++;
        }

        /**
         * @brief Returns whether this timer should accumulate time.
         *
         * @return True if the timer is in the Running state.
         */
        [[nodiscard]] bool shouldUpdate() const noexcept {
            return (timerState_ == TimerState::Running);
        }

        /**
         * @brief Returns whether this timer's internal state represents the Finished-state.
         *
         * @return True if the timer is finished.
         */
        [[nodiscard]] bool isFinished() const noexcept {
            return (timerState_ == TimerState::Finished);
        }

        /**
         * @brief Returns the current revision counter.
         *
         * @return The TimerRevision value.
         */
        [[nodiscard]] TimerRevision timerRevision() const noexcept {
            return timerRevision_;
        }

        /**
         * @brief Advances the timer by the given frame time.
         *
         * Only accumulates time when the timer is in the Running state.
         * Increments the revision counter on each successful update.
         *
         * @param ft Frame time in seconds.
         */
        void update(const float ft) {
            if (!shouldUpdate() || ft == 0.0f) {
                return;
            }
            timerRevision_++;
            elapsed_ += ft;
        }

    };

}
