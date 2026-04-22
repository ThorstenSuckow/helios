/**
 * @file TimerRevision.ixx
 * @brief Type alias for timer revision counters.
 */
module;

#include <cstdint>

export module helios.runtime.timing.types.TimerRevision;


export namespace helios::runtime::timing::types {

    /**
     * @brief Monotonically increasing revision counter for Timer updates.
     */
    using TimerRevision = uint32_t;

}