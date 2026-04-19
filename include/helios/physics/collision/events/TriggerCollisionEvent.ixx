/**
 * @file TriggerCollisionEvent.ixx
 * @brief Event emitted when a trigger (non-physical) collision occurs.
 */
module;

export module helios.physics.collision.events.TriggerCollisionEvent;

import helios.ecs.types.EntityHandle;
import helios.math;
import helios.physics.collision.types.CollisionContext;
import helios.core.types;

export namespace helios::physics::collision::events {

    /**
     * @brief Event emitted when a trigger collision is detected.
     *
     * Trigger collisions are non-physical interactions used for gameplay logic
     * such as pickups, damage zones, or area effects. Unlike solid collisions,
     * trigger collisions do not block movement. This event is published to the
     * event bus when an entity with PassEvent behavior detects a trigger collision.
     */
    template<typename THandle>
    class TriggerCollisionEvent {

        /**
         * @brief Context data describing the collision.
         */
        collision::types::CollisionContext<THandle> collisionContext_;

        /**
         * @brief Handle of the entity that reported the collision.
         */
        THandle source_;

    public:

        /**
         * @brief Constructs a TriggerCollisionEvent.
         *
         * @param source Handle of the reporting entity.
         * @param collisionContext Context data describing the collision.
         */
        explicit TriggerCollisionEvent(
            const THandle source,
            const collision::types::CollisionContext<THandle>& collisionContext
        ) : collisionContext_(collisionContext), source_(source) {}

        /**
         * @brief Returns the collision context.
         *
         * @return Reference to the collision context data.
         */
        [[nodiscard]] const collision::types::CollisionContext<THandle>& collisionContext() const noexcept {
            return collisionContext_;
        }

        /**
         * @brief Returns the source entity handle.
         *
         * @return Handle of the entity that reported the collision.
         */
        [[nodiscard]] THandle source() const noexcept {
            return source_;
        }

    };

}
