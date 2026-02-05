/**
 * @file CollisionLayer.ixx
 * @brief Type definitions for collision layer identifiers.
 */
module;

#include <cstdint>
#include <limits>

export module helios.engine.core.data.CollisionLayer;



export namespace helios::engine::core::data {

    /**
     * @brief Type alias for collision layer identifiers.
     *
     * Collision layers are represented as bitmask values where each layer
     * is a single bit. This allows efficient layer masking operations.
     */
    using CollisionLayer = uint32_t;

    /**
     * @brief Maximum number of distinct collision layers.
     *
     * Derived from the number of bits in CollisionLayer (32 for uint32_t).
     */
    constexpr std::size_t MAX_COLLISION_LAYERS = std::numeric_limits<CollisionLayer>::digits;


}