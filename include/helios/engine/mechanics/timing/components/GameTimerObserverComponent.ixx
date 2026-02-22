/**
 * @file GameTimerObserverComponent.ixx
 * @brief Component that observes a specific game timer.
 */
module;

export module helios.engine.mechanics.timing.components.GameTimerObserverComponent;

import helios.engine.mechanics.timing.types;

import helios.engine.core.data;
import helios.core.types;

namespace helios::engine::mechanics::timing::systems {
    class GameTimerObserverClearSystem {};
}

export namespace helios::engine::mechanics::timing::components {

    using namespace helios::engine::core::data;
    using namespace helios::engine::mechanics::timing::types;

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
    class GameTimerObserverComponent {

        friend class helios::engine::mechanics::timing::systems::GameTimerObserverClearSystem;

    private:

        /**
         * @brief The id of the observed game timer.
         */
        GameTimerId gameTimerId_;

        /**
         * @brief Cached elapsed time from the last observation.
         */
        float elapsed_;

        /**
         * @brief Last known revision of the observed timer.
         */
        TimerRevision timerRevision_;

        /**
         * @brief True if the observed timer revision changed since the last clear.
         */
        bool hasUpdate_ = false;

        /**
         * @brief Resets the update flag.
         *
         * Called by GameTimerObserverClearSystem after the update has been consumed.
         */
        void clearUpdate() noexcept {
            hasUpdate_ = false;
        }

    public:


        GameTimerObserverComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        GameTimerObserverComponent(const GameTimerObserverComponent& other) :
            gameTimerId_(other.gameTimerId_) {}

        GameTimerObserverComponent& operator=(const GameTimerObserverComponent&) = default;
        GameTimerObserverComponent(GameTimerObserverComponent&&) noexcept = default;
        GameTimerObserverComponent& operator=(GameTimerObserverComponent&&) noexcept = default;

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
            hasUpdate_ = true;
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

        /**
         * @brief Indicates whether the observed timer revision has changed.
         *
         * @return True if the revision changed since the last clear.
         */
        [[nodiscard]] bool hasUpdate() const noexcept {
            return hasUpdate_;
        }

    };


}