/**
 * @file MaxScoreObserverComponent.ixx
 * @brief Component for observing score pool changes as high score.
 */
module;


export module helios.engine.mechanics.scoring.components.MaxScoreObserverComponent;

import helios.engine.mechanics.scoring.MaxScorePoolSnapshot;
import helios.engine.mechanics.scoring.types.ScorePoolRevision;

import helios.engine.core.data;
import helios.core.types;

namespace helios::engine::mechanics::scoring::systems {
    class MaxScoreObserverClearSystem {};
}

export namespace helios::engine::mechanics::scoring::components {


    /**
     * @brief Component that observes and caches the high score value from a ScorePool.
     *
     * Attached to UI entities to display score values. The ScoreObserverSystem
     * updates this component's value from the referenced ScorePool each frame.
     * The hasUpdate flag indicates when the value has changed.
     */
    class MaxScoreObserverComponent {


        friend class helios::engine::mechanics::scoring::systems::MaxScoreObserverClearSystem;

    private:

        /**
         * @brief ID of the score pool to observe.
         */
        helios::engine::core::data::ScorePoolId scorePoolId_{helios::core::types::no_init};


        /**
         * @brief Flag indicating whether the value changed this frame.
         *
         * Cleared by ScoreObserverClearSystem at frame end.
         */
        bool hasUpdate_ = true;

        /**
         * @brief the current snapshot saved by this observer component.
         */
        helios::engine::mechanics::scoring::MaxScorePoolSnapshot snapshot_;

        /**
         * @brief Whether this component is enabled.
         */
        bool isEnabled_ = true;

        /**
         * @brief Clears the update flag.
         *
         * Called by ScoreObserverClearSystem.
         */
        void clearUpdate() {
            hasUpdate_ = false;
        }

    public:

        /**
         * @brief Checks whether this component is enabled.
         *
         * @return True if enabled, false otherwise.
         */
        [[nodiscard]] bool isEnabled() const noexcept {
            return isEnabled_;
        }

        /**
         * @brief Enables this component.
         */
        void enable() noexcept {
            isEnabled_ = true;
        }

        /**
         * @brief Disables this component.
         */
        void disable() noexcept {
            isEnabled_ = false;
        }

        MaxScoreObserverComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        MaxScoreObserverComponent(const MaxScoreObserverComponent& other) :
            scorePoolId_(other.scorePoolId_) {}

        MaxScoreObserverComponent& operator=(const MaxScoreObserverComponent&) = default;
        MaxScoreObserverComponent(MaxScoreObserverComponent&&) noexcept = default;
        MaxScoreObserverComponent& operator=(MaxScoreObserverComponent&&) noexcept = default;

        /**
         * @brief Sets the score pool to observe.
         *
         * @param scorePoolId The ID of the pool to observe.
         */
        void setScorePoolId(const helios::engine::core::data::ScorePoolId scorePoolId) noexcept {
            scorePoolId_ = scorePoolId;
        }

        /**
         * @brief Returns the observed score pool ID.
         *
         * @return The ScorePoolId.
         */
        [[nodiscard]] helios::engine::core::data::ScorePoolId scorePoolId() const noexcept {
            return scorePoolId_;
        }

        /**
         * @brief Sets the cached snapshot value.
         *
         * Sets hasUpdate to true if the value changes.
         *
         * @param snapshot The snapshot to use for the next update..
         */
        void update(const helios::engine::mechanics::scoring::MaxScorePoolSnapshot& snapshot) noexcept {
            if (snapshot_.revision == snapshot.revision) {
                return;
            }
            snapshot_ = snapshot;
            hasUpdate_ = true;
        }

        /**
         * @brief Returns the cached max score.
         *
         * @return The current score value.
         */
        [[nodiscard]] double maxScore() const noexcept {
            return snapshot_.maxScore;
        }

        /**
         * @brief Checks if the value changed since last clear.
         *
         * @return True if the value was updated this frame.
         */
        [[nodiscard]] bool hasUpdate() const noexcept {
            return hasUpdate_;
        }


    };


}