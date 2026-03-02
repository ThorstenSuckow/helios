/**
 * @file LivesRevision.ixx
 * @brief Type alias for the lives revision counter.
 */
module;

#include <cstdint>

export module helios.engine.mechanics.match.types.LivesRevision;


export namespace helios::engine::mechanics::match::types {

    /**
     * @brief Monotonically increasing counter tracking LivesComponent mutations.
     *
     * Observer systems compare their cached revision against the
     * component's current revision to detect changes.
     */
    using LivesRevision = uint32_t;

}