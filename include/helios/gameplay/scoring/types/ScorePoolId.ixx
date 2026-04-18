/**
* @file ScorePoolId.ixx
 * @brief Strongly-typed identifier for ScorePools.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.gameplay.scoring.types.ScorePoolId;

import helios.core.types.StrongId;

export namespace helios::gameplay::scoring::types {

    /**
     * @brief Tag type for ScorePoolId.
     */
    struct ScorePoolIdTag{};

    /**
     * @brief Strongly-typed identifier for score pools.
     *
     * @details Used to uniquely identify score pools.
     *
     * @see helios::core::types::StrongId
     */
    using ScorePoolId = helios::core::types::StrongId<ScorePoolIdTag>;

}
