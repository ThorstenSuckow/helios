/**
 * @file ScorePool.ixx
 * @brief Container for accumulated scores by type.
 */
module;


#include <stdexcept>
#include <unordered_map>

export module helios.engine.mechanics.scoring.ScorePool;

import helios.engine.mechanics.scoring.ScorePoolSnapshot;

import helios.engine.mechanics.scoring.types;

import helios.engine.core.data.ScorePoolId;

import helios.core.types;

export namespace helios::engine::mechanics::scoring {

    /**
     * @brief Container that accumulates scores by type within a pool.
     *
     * @details ScorePool maintains a collection of scores indexed by ScoreTypeId,
     * along with a running total. It tracks changes via a revision number that
     * is incremented whenever a score value changes, enabling efficient change
     * detection by observers.
     *
     * ScorePools are managed by ScorePoolManager, which handles command processing
     * and pool lifecycle.
     *
     * @see ScorePoolManager
     * @see ScorePoolSnapshot
     * @see ScoreValueContext
     */
    class ScorePool {

        /**
         * @brief Unique identifier for this score pool.
         */
        helios::engine::core::data::ScorePoolId scorePoolId_;

        /**
         * @brief Per-type score values indexed by ScoreTypeId.
         */
        std::vector<double> scores_;

        /**
         * @brief Running total of all scores in this pool.
         */
        double totalScore_{};

        /**
         * @brief Adds to the running total.
         *
         * @param total Value to add.
         */
        void addTotal(const double total) noexcept {
            totalScore_ += total;
        }

        /**
         * @brief Sets the running total directly.
         *
         * @param total New total value.
         */
        void setTotal(const double total) noexcept {
            totalScore_ = total;
        }

        /**
         * @brief The current revision of this ScorePool.
         */
        helios::engine::mechanics::scoring::types::ScorePoolRevision revision_{};

    public:

        /**
         * @brief Constructs a ScorePool with the given ID.
         *
         * @param scorePoolId Unique identifier for this pool.
         */
        explicit ScorePool(const helios::engine::core::data::ScorePoolId scorePoolId)
            : scorePoolId_{scorePoolId} {}

        /**
         * @brief Returns the pool's unique identifier.
         *
         * @return The ScorePoolId.
         */
        [[nodiscard]] helios::engine::core::data::ScorePoolId id() const noexcept {
            return scorePoolId_;
        }

        /**
         * @brief Adds a score from the given context.
         *
         * Updates both the per-type score and the running total.
         *
         * @param scoreContext Context containing type ID and value.
         */
        void addScore(const helios::engine::mechanics::scoring::types::ScoreValueContext& scoreContext) {

            auto id = scoreContext.scoreTypeId.value();

            if (id >= scores_.size()) {
                scores_.resize(id + 1, 0.0f);
            }

            const double oldScore = scores_[id];
            scores_[id] += scoreContext.value;
            if (oldScore != scores_[id]) {
                revision_++;
            }
            addTotal(scoreContext.value);
        }

        /**
         * @brief Resets all scores to zero.
         */
        void reset() noexcept {
            std::fill(scores_.begin(), scores_.end(), 0.0f);
            setTotal(0.0f);
            revision_++;
        }


        /**
         * @brief Returns the running total of all scores.
         *
         * @return The total score value.
         */
        [[nodiscard]] double totalScore() const noexcept {
            return totalScore_;
        }

        /**
         * @brief Returns the pool's unique identifier.
         *
         * @return The ScorePoolId.
         */
        [[nodiscard]] helios::engine::core::data::ScorePoolId scorePoolId() const noexcept {
            return scorePoolId_;
        }

        /**
         * @brief Returns the current revision number.
         *
         * @details The revision is incremented whenever a score value changes.
         * Used by observers to detect changes without polling values.
         *
         * @return The current ScorePoolRevision.
         */
        [[nodiscard]] helios::engine::mechanics::scoring::types::ScorePoolRevision revision() const noexcept {
            return revision_;
        }

        /**
         * @brief Returns a snapshot of the current pool state.
         *
         * @details The snapshot contains the pool ID, total score, and current
         * revision. Useful for observers and UI binding.
         *
         * @return A ScorePoolSnapshot with current values.
         */
        [[nodiscard]] helios::engine::mechanics::scoring::ScorePoolSnapshot snapshot() const noexcept {
            return {.scorePoolId = scorePoolId_, .totalScore = totalScore_, .revision = revision_};
        }



    };

}
