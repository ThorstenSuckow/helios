/**
 * @file TimerRevision.ixx
 * @brief Type alias for timer revision counters.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.timing.types.TimerRevision;


export namespace helios::engine::mechanics::timing::types {

    /**
     * @brief Monotonically increasing revision counter for GameTimer updates.
     */
    using TimerRevision = uint32_t;

}