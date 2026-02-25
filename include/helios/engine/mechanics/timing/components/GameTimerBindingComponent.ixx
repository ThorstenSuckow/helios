/**
 * @file GameTimerBindingComponent.ixx
 * @brief Component that observes a specific game timer.
 */
module;

export module helios.engine.mechanics.timing.components.GameTimerBindingComponent;

import helios.engine.mechanics.timing.types;

import helios.engine.core.data;
import helios.core.types;

using namespace helios::engine::core::data;
using namespace helios::engine::mechanics::timing::types;

export namespace helios::engine::mechanics::timing::components {

    /**
     * @brief Component that tracks the revision of a specific GameTimer.
     *
     * Entities with this component observe a GameTimer identified by its
     * GameTimerId. The stored revision can be compared against the timer's
     * current revision to detect updates.
     *
     * @see GameTimer
     * @see GameTimerUpdateSystem
     */
    class GameTimerBindingComponent {

    private:

        /**
         * @brief The id of the observed game timer.
         */
        GameTimerId gameTimerId_;

        /**
         * @brief Last known revision of the observed timer.
         */
        TimerRevision timerRevision_;


    public:


        GameTimerBindingComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        GameTimerBindingComponent(const GameTimerBindingComponent& other) :
            gameTimerId_(other.gameTimerId_) {}

        GameTimerBindingComponent& operator=(const GameTimerBindingComponent&) = default;
        GameTimerBindingComponent(GameTimerBindingComponent&&) noexcept = default;
        GameTimerBindingComponent& operator=(GameTimerBindingComponent&&) noexcept = default;

        /**
         * @brief Sets the id of the game timer to observe.
         *
         * @param gameTimerId The timer id.
         */
        void setGameTimerId(const helios::engine::core::data::GameTimerId gameTimerId) noexcept {
            gameTimerId_ = gameTimerId;
        }

        /**
         * @brief Returns the observed game timer id.
         *
         * @return The GameTimerId.
         */
        [[nodiscard]] helios::engine::core::data::GameTimerId gameTimerId() const noexcept {
            return gameTimerId_;
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