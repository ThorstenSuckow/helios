/**
 * @file ScoreObserverComponent.ixx
 * @brief Component for observing score pool changes.
 */
module;


export module helios.engine.mechanics.scoring.components.ScoreObserverComponent;

import helios.engine.mechanics.scoring.ScorePoolSnapshot;
import helios.engine.mechanics.scoring.types.ScorePoolRevision;

import helios.engine.ecs.CloneableComponent;

import helios.engine.core.data;
import helios.core.types;

namespace helios::engine::mechanics::scoring::systems {
    class ScoreObserverClearSystem {};
}

export namespace helios::engine::mechanics::scoring::components {


    /**
     * @brief Component that observes and caches the score value from a ScorePool.
     *
     * Attached to UI entities to display score values. The ScoreObserverSystem
     * updates this component's value from the referenced ScorePool each frame.
     * The hasUpdate flag indicates when the value has changed.
     */
    class ScoreObserverComponent : public helios::engine::ecs::CloneableComponent<ScoreObserverComponent> {

        friend class helios::engine::mechanics::scoring::systems::ScoreObserverClearSystem;

    private:

        /**
         * @brief ID of the score pool to observe.
         */
        helios::engine::core::data::ScorePoolId scorePoolId_{helios::core::types::no_init};

        /**
         * @brief Cached generation id of the ScorePoolSnapshot.
         */
        helios::engine::mechanics::scoring::types::ScorePoolRevision revision_;

        /**
         * @brief Flag indicating whether the value changed this frame.
         *
         * Cleared by ScoreObserverClearSystem at frame end.
         */
        bool hasUpdate_ = true;

        /**
         * @brief the current snapshot saved by this observer component.
         */
        helios::engine::mechanics::scoring::ScorePoolSnapshot snapshot_;

        /**
         * @brief Clears the update flag.
         *
         * Called by ScoreObserverClearSystem.
         */
        void clearUpdate() {
            hasUpdate_ = false;
        }

    public:


        ScoreObserverComponent() = default;

        /**
         * @brief Copy constructor.
         *
         * @param other The component to copy from.
         */
        ScoreObserverComponent(const ScoreObserverComponent& other) :
            scorePoolId_(other.scorePoolId_) {}

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
        void update(const helios::engine::mechanics::scoring::ScorePoolSnapshot& snapshot) noexcept {
            if (revision_ == snapshot.revision) {
                return;
            }
            snapshot_ = snapshot;
            hasUpdate_ = true;
        }

        /**
         * @brief Returns the cached total score.
         *
         * @return The current score value.
         */
        [[nodiscard]] double totalScore() const noexcept {
            return snapshot_.totalScore;
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