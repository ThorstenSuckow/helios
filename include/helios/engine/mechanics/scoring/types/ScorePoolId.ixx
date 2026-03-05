/**
* @file ScorePoolId.ixx
 * @brief Strongly-typed identifier for ScorePools.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.mechanics.scoring.types.ScorePoolId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::mechanics::scoring::types {

    /**
     * @brief Tag type for ScorePoolId.
     */
    struct ScorePoolIdTag{};

    /**
     * @brief Strongly-typed identifier for score pools.
     *
     * @details Used to uniquely identify score pools.
     *
     * @see helios::core::data::StrongId
     */
    using ScorePoolId = helios::core::data::StrongId<ScorePoolIdTag, uint32_t>;

}
