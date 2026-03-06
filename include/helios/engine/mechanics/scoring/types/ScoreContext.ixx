/**
 * @file ScoreContext.ixx
 * @brief Data structure for score update information.
 */
module;

export module helios.engine.mechanics.scoring.types.ScoreValueContext;


import helios.engine.mechanics.scoring.types.ScoreTypeId;
import helios.engine.mechanics.scoring.types.ScorePoolId;
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
        ScoreTypeId scoreTypeId;

        /**
         * @brief Target score pool to receive this score.
         */
        ScorePoolId scorePoolId;

        /**
         * @brief The score value to add.
         */
        double value{};

    };

}
