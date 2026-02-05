/**
 * @file CollisionBehavior.ixx
 * @brief Enumeration of collision response behaviors.
 */
module;

#include <cstdint>

export module helios.engine.modules.physics.collision.types.CollisionBehavior;



export namespace helios::engine::modules::physics::collision::types {

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
    enum class CollisionBehavior : uint16_t {

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
        PassEvent   = 1 << 4,

        /**
         * @brief Push collision event to the event bus.
         *
         * @details Pushes the collision event to the double-buffered event bus
         * for processing in the next frame. Used for deferred collision handling.
         */
        PushEvent   = 1 << 5,

        /**
         * @brief Process collision event in the current frame.
         *
         * @details Sends the collision event to the immediate bus for processing
         * within the same frame. Used for time-critical collision responses.
         */
        FrameEvent   = 1 << 6
    };

    /**
     * @brief Number of distinct CollisionBehavior flags.
     */
    constexpr size_t CollisionBehaviorItemSize = 8;

    /**
     * @brief Combines two CollisionBehavior flags using bitwise OR.
     *
     * @param lhs Left-hand side behavior.
     * @param rhs Right-hand side behavior.
     *
     * @return Combined behavior flags.
     */
    [[nodiscard]] constexpr CollisionBehavior operator|(CollisionBehavior lhs, CollisionBehavior rhs) noexcept {
        return static_cast<CollisionBehavior>(
            static_cast<uint16_t>(lhs) | static_cast<uint16_t>(rhs)
        );
    }

    /**
     * @brief Masks two CollisionBehavior flags using bitwise AND.
     *
     * @param lhs Left-hand side behavior.
     * @param rhs Right-hand side behavior.
     *
     * @return Masked behavior flags.
     */
    [[nodiscard]] constexpr CollisionBehavior operator&(CollisionBehavior lhs, CollisionBehavior rhs) noexcept {
        return static_cast<CollisionBehavior>(
            static_cast<uint16_t>(lhs) & static_cast<uint16_t>(rhs)
        );
    }

    /**
     * @brief Checks if a behavior mask contains a specific flag.
     *
     * @param mask The behavior mask to check.
     * @param flag The flag to test for.
     *
     * @return True if the mask contains the flag, false otherwise.
     */
    [[nodiscard]] constexpr bool hasFlag(const CollisionBehavior mask, const CollisionBehavior flag) noexcept {
        return (mask & flag) == flag;
    }

}

