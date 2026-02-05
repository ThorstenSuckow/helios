/**
 * @file ScorePoolRevision.ixx
 * @brief Type alias for score pool revision tracking.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.scoring.types.ScorePoolRevision;


export namespace helios::engine::mechanics::scoring::types {

    /**
     * @brief Type alias for tracking score pool revisions.
     *
     * Used to detect changes in a ScorePool. Incremented each time
     * the pool's score changes, allowing observers to efficiently
     * check for updates.
     */
    using ScorePoolRevision = uint32_t;

}