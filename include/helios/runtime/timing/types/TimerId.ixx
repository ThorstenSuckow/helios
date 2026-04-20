/**
* @file TimerId.ixx
 * @brief Strongly-typed identifier for timers.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.runtime.timing.types.TimerId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::runtime::timing::types {

    /**
     * @brief Tag type for TimerId.
     */
    struct TimerIdTag{};

    /**
     * @brief Strongly-typed identifier for timers.
     *
     * @details Used to uniquely identify timers.
     *
     * @see helios::core::types::StrongId
     */
    using TimerId = helios::core::types::StrongId<TimerIdTag>;

}
