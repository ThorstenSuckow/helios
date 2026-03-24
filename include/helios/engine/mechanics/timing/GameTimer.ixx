/**
 * @file GameTimer.ixx
 * @brief A game timer that tracks elapsed time and supports state transitions.
 */
module;

export module helios.engine.mechanics.timing.GameTimer;

import helios.engine.mechanics.timing.types.GameTimerId;

import helios.engine.mechanics.timing.types;

using namespace helios::engine::mechanics::timing::types;
using namespace helios::engine::mechanics::timing::types;

export namespace helios::engine::mechanics::timing {

    /**
     * @brief A game timer identified by a GameTimerId.
     *
     * Tracks elapsed time while in the Started state. Each update increments
     * a revision counter that observers can use to detect changes.
     *
     * @see TimerManager
     * @see GameTimerBindingComponent
     */
    class GameTimer {

        /**
         * @brief Monotonically increasing revision counter, incremented on each update.
         */
        TimerRevision timerRevision_{};

        /**
         * @brief Unique identifier for this timer.
         */
        GameTimerId gameTimerId_;

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
         * @brief Constructs a GameTimer with the given identifier.
         *
         * @param gameTimerId The unique identifier for this timer.
         */
        explicit GameTimer(const helios::engine::mechanics::timing::types::GameTimerId gameTimerId)
            : gameTimerId_{gameTimerId} {}

        /**
         * @brief Returns the timer identifier.
         *
         * @return The GameTimerId assigned to this timer.
         */
        [[nodiscard]] helios::engine::mechanics::timing::types::GameTimerId gameTimerId() const noexcept {
            return gameTimerId_;
        }

        /**
         * @brief Resets the elapsed time to zero and sets timer state to Undefined.
         *
         * @param state The new TimerState the timer should be resetted to.
         */
        void reset(const TimerState state = TimerState::Undefined) noexcept {
            elapsed_ = 0.0f;
            timerState_ = state;
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
         * @return True if the timer is in the Started state.
         */
        [[nodiscard]] bool shouldUpdate() const noexcept {
            return (timerState_ == TimerState::Started);
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
         * Only accumulates time when the timer is in the Started state.
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
