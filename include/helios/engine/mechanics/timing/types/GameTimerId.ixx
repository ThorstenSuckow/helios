/**
* @file GameTimerId.ixx
 * @brief Strongly-typed identifier for gameTimers.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.mechanics.timing.types.GameTimerId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

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
     * @see helios::core::types::StrongId
     */
    using GameTimerId = helios::core::types::StrongId<GameTimerIdTag>;

}
