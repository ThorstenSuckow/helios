/**
 * @file TimerRevision.ixx
 * @brief Type alias for timer revision counters.
 */
module;

#include <cstdint>

export module helios.gameplay.timing.types.TimerRevision;


export namespace helios::gameplay::timing::types {

    /**
     * @brief Monotonically increasing revision counter for GameTimer updates.
     */
    using TimerRevision = uint32_t;

}