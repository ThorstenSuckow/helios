/**
 * @file CollisionBehavior.ixx
 * @brief Enumeration of collision response behaviors.
 */
module;

#include <cstdint>

export module helios.engine.game.physics.collision.types.CollisionBehavior;



export namespace helios::engine::game::physics::collision::types {

    /**
     * @brief Defines how an entity responds to a collision event.
     *
     * @details CollisionBehavior values are used as bitmasks to configure
     * per-layer collision responses in CollisionComponent. Multiple behaviors
     * can be combined using bitwise operations.
     *
     * The behavior determines what action the collision system takes when
     * two entities collide based on their layer configuration.
     *
     * @note Not all behaviors are currently fully implemented. Currently supported:
     * - `Reflect` — Fully supported
     * - `Bounce` — Fully supported
     * - `Despawn` — Fully supported
     */
    enum class CollisionBehavior : uint8_t {

        /**
         * @brief No collision response.
         */
        None    = 0,

        /**
         * @brief Reflect the entity's velocity off the collision surface.
         *
         * @details Uses the surface normal to calculate a reflected velocity
         * vector. Commonly used for projectiles bouncing off walls.
         */
        Reflect = 1 << 0,

        /**
         * @brief Bounce the entity with restitution applied.
         *
         * @details Similar to Reflect but applies a restitution coefficient
         * to reduce velocity. Used for physics-based bouncing.
         */
        Bounce  = 1 << 1,

        /**
         * @brief Stick the entity to the collision surface.
         *
         * @details Stops the entity's movement and attaches it to the
         * collided object. Used for projectiles that embed on impact.
         */
        Stick   = 1 << 2,

        /**
         * @brief Despawn the entity on collision.
         *
         * @details Marks the entity for removal from the game world.
         * Used for projectiles that are destroyed on impact.
         */
        Despawn = 1 << 3,

        /**
         * @brief Emit an event without physical response.
         *
         * @details Generates a collision event for game logic processing
         * without affecting the entity's physics state.
         */
        Event   = 1 << 4,

        /**
         * @brief Internal: Number of behavior types for array sizing.
         *
         * @warning Do not use as a behavior value.
         */
        size_ = 6
    };



}