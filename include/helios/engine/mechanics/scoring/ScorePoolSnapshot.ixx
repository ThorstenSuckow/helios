/**
 * @file ScorePoolSnapshot.ixx
 * @brief Immutable snapshot of a score pool's state.
 */
module;

export module helios.engine.mechanics.scoring.ScorePoolSnapshot;

import helios.core;
import helios.engine.core.data;
import helios.engine.mechanics.scoring.types.ScorePoolRevision;



export namespace helios::engine::mechanics::scoring {


    /**
     * @brief Immutable snapshot of a ScorePool's state.
     *
     * @details Captures the pool ID, total score, and revision at a specific
     * point in time. Useful for passing score data to UI or other systems
     * without exposing the mutable ScorePool.
     *
     * The revision field allows observers to detect changes by comparing
     * snapshots without needing to track individual score values.
     */
    struct ScorePoolSnapshot {

        /**
         * @brief Unique identifier of the score pool.
         */
        helios::engine::core::data::ScorePoolId scorePoolId{helios::core::types::no_init};

        /**
         * @brief The total score value at the time of the snapshot.
         */
        double totalScore;

        /**
         * @brief The revision number for change detection.
         *
         * @details Incremented whenever a score in the pool changes.
         * Compare revisions to detect updates without polling values.
         */
        helios::engine::mechanics::scoring::types::ScorePoolRevision revision;
    };

}

