/**
* @file GameTimerId.ixx
 * @brief Strongly-typed identifier for gameTimers.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.mechanics.timing.types.GameTimerId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::mechanics::timing::types {

    /**
     * @brief Tag type for GameTimerId.
     */
    struct GameTimerIdTag{};

    /**
     * @brief Strongly-typed identifier for gameTimers.
     *
     * @details Used to uniquely identify gameTimers.
     *
     * @see helios::core::data::StrongId
     */
    using GameTimerId = helios::core::data::StrongId<GameTimerIdTag, uint32_t>;

}
