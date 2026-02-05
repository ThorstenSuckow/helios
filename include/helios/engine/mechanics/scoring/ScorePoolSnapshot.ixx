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
     * Captures the pool ID and current score value at a specific point in time.
     * Useful for passing score data to UI or other systems without exposing
     * the mutable ScorePool.
     */
    struct ScorePoolSnapshot {

        /**
         * @brief Unique identifier of the score pool.
         */
        helios::engine::core::data::ScorePoolId scorePoolId{helios::core::types::no_init};

        /**
         * @brief The score value at the time of the snapshot.
         */
        double totalScore;

        /**
         * @brief The revision of the ScorePool.
         */
        helios::engine::mechanics::scoring::types::ScorePoolRevision revision;
    };

}

