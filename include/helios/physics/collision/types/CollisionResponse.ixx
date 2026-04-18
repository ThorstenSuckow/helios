/**
 * @file CollisionResponse.ixx
 * @brief Enum for defining collision response behaviors.
 */
module;

#include <cstdint>

export module helios.physics.collision.types.CollisionResponse;



export namespace helios::physics::collision::types {

    /**
     * @brief Defines how an entity responds to a collision event.
     *
     * @details CollisionResponse is a bitmask enum that specifies the behavior
     * when a collision is detected.
     *
     * @see CollisionComponent
     */
    enum class CollisionResponse : uint8_t {

        /**
         * @brief No collision response; collision is ignored.
         */
        None = 0,

        /**
         * @brief Aligns the entity's heading direction upon collision.
         *
         * @details Aligns the heading of the object to the the movement direction, typically used for bouncing or reflecting projectiles off surfaces.
         */
        AlignHeadingToDirection = 1 << 0,

        /**
         * @brief Sentinel value indicating the number of response types.
         */
        size_ = 2
    };



}