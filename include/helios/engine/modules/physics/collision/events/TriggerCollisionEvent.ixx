/**
 * @file TriggerCollisionEvent.ixx
 * @brief Event emitted when a trigger (non-physical) collision occurs.
 */
module;

export module helios.engine.modules.physics.collision.events.TriggerCollisionEvent;

import helios.util.Guid;
import helios.math;
import helios.engine.modules.physics.collision.types.CollisionContext;
import helios.core.types;

export namespace helios::engine::modules::physics::collision::events {

    /**
     * @brief Event emitted when a trigger collision is detected.
     *
     * Trigger collisions are non-physical interactions used for gameplay logic
     * such as pickups, damage zones, or area effects. Unlike solid collisions,
     * trigger collisions do not block movement. This event is published to the
     * event bus when an entity with PassEvent behavior detects a trigger collision.
     */
    class TriggerCollisionEvent {

        /**
         * @brief Context data describing the collision.
         */
        collision::types::CollisionContext collisionContext_;

        /**
         * @brief GUID of the entity that reported the collision.
         */
        helios::util::Guid source_;

    public:

        /**
         * @brief Constructs a TriggerCollisionEvent.
         *
         * @param source GUID of the reporting entity.
         * @param collisionContext Context data describing the collision.
         */
        explicit TriggerCollisionEvent(
            const helios::util::Guid source,
            const collision::types::CollisionContext& collisionContext
        ) : collisionContext_(collisionContext), source_(source) {}

        /**
         * @brief Returns the collision context.
         *
         * @return Reference to the collision context data.
         */
        [[nodiscard]] const collision::types::CollisionContext& collisionContext() const noexcept {
            return collisionContext_;
        }

        /**
         * @brief Returns the source entity GUID.
         *
         * @return GUID of the entity that reported the collision.
         */
        [[nodiscard]] helios::util::Guid source() const noexcept {
            return source_;
        }

    };

}
