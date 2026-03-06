/**
 * @file MaxScorePoolSnapshot.ixx
 * @brief Immutable snapshot of a ScorePool's high score state.
 */
module;

export module helios.engine.mechanics.scoring.MaxScorePoolSnapshot;

import helios.core;
import helios.engine.mechanics.scoring.types;



export namespace helios::engine::mechanics::scoring {


    /**
     * @brief Immutable snapshot of a ScorePool's high score state.
     *
     * Captures the maximum score value and its associated revision at a
     * specific point in time. Used by MaxScoreObserverComponent to detect
     * changes without polling the ScorePool directly.
     *
     * @see ScorePool::maxScoreSnapshot
     * @see MaxScoreObserverComponent
     */
    struct MaxScorePoolSnapshot {

        /**
         * @brief ID of the source score pool.
         */
        helios::engine::mechanics::scoring::types::ScorePoolId scorePoolId{helios::core::types::no_init};

        /**
         * @brief The maximum score value at snapshot time.
         */
        double maxScore;

        /**
         * @brief Revision counter for change detection.
         */
        helios::engine::mechanics::scoring::types::ScorePoolRevision revision;
    };

}

