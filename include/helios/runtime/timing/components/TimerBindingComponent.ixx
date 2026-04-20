/**
 * @file TimerBindingComponent.ixx
 * @brief Component that observes a specific game timer.
 */
module;

export module helios.runtime.timing.components.TimerBindingComponent;

import helios.runtime.timing.types;

import helios.runtime.timing.types.TimerId;
import helios.core.types;

using namespace helios::runtime::timing::types;
using namespace helios::runtime::timing::types;

export namespace helios::runtime::timing::components {

    /**
     * @brief Component that tracks the revision of a specific Timer.
     *
     * Entities with this component observe a Timer identified by its
     * TimerId. The stored revision can be compared against the timer's
     * current revision to detect updates.
     *
     * @see Timer
     * @see TimerUpdateSystem
     */
    template<typename THandle>
    class TimerBindingComponent {

    private:

        /**
         * @brief The id of the observed game timer.
         */
        TimerId timerId_;

        /**
         * @brief Last known revision of the observed timer.
         */
        TimerRevision timerRevision_;


    public:


        TimerBindingComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        TimerBindingComponent(const TimerBindingComponent& other) :
            timerId_(other.timerId_) {}

        TimerBindingComponent& operator=(const TimerBindingComponent&) = default;
        TimerBindingComponent(TimerBindingComponent&&) noexcept = default;
        TimerBindingComponent& operator=(TimerBindingComponent&&) noexcept = default;

        /**
         * @brief Sets the id of the game timer to observe.
         *
         * @param timerId The timer id.
         */
        void setTimerId(const helios::runtime::timing::types::TimerId timerId) noexcept {
            timerId_ = timerId;
        }

        /**
         * @brief Returns the observed game timer id.
         *
         * @return The TimerId.
         */
        [[nodiscard]] helios::runtime::timing::types::TimerId timerId() const noexcept {
            return timerId_;
        }

        /**
         * @brief Sets the last known timer revision.
         *
         * If the new revision differs from the stored one, the update flag
         * is set so that consuming systems can detect the change.
         *
         * @param timerRevision The revision value.
         */
        void setTimerRevision(const TimerRevision timerRevision) noexcept {
            if (timerRevision_ == timerRevision) {
                return;
            }
            timerRevision_ = timerRevision;
        }

        /**
         * @brief Returns the last known timer revision.
         *
         * @return The stored TimerRevision.
         */
        [[nodiscard]] TimerRevision timerRevision() const noexcept {
            return timerRevision_;
        }

    };


}