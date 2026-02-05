/**
 * @file ScoreValueContext.ixx
 * @brief Data structure for score update information.
 */
module;

export module helios.engine.mechanics.scoring.types.ScoreValueContext;

import helios.engine.core.data;
import helios.core.types;

export namespace helios::engine::mechanics::scoring::types {

    /**
     * @brief Data structure containing score update information.
     *
     * Encapsulates the score type, target pool, and value for a score update.
     * Used by UpdateScoreCommand to transfer scoring data.
     */
    struct ScoreValueContext {

        /**
         * @brief Type identifier for the score (e.g., KillReward).
         */
        helios::engine::core::data::ScoreTypeId scoreTypeId;

        /**
         * @brief Target score pool to receive this score.
         */
        helios::engine::core::data::ScorePoolId scorePoolId;

        /**
         * @brief The score value to add.
         */
        double value{};

    };

}
