/**
 * @file SolidCollisionEvent.ixx
 * @brief Event emitted when a solid (physical) collision occurs.
 */
module;

export module helios.engine.modules.physics.collision.events.SolidCollisionEvent;

import helios.engine.ecs.EntityHandle;
import helios.math;
import helios.engine.modules.physics.collision.types.CollisionContext;
import helios.core.types;


export namespace helios::engine::modules::physics::collision::events {

    /**
     * @brief Event emitted when a solid collision is detected.
     *
     * Solid collisions represent physical interactions between entities that can
     * block movement or trigger physics responses. This event is published to the
     * event bus when an entity with PassEvent behavior detects a solid collision.
     */
    class SolidCollisionEvent {

        /**
         * @brief Context data describing the collision.
         */
        collision::types::CollisionContext collisionContext_;

        /**
         * @brief Handle of the entity that reported the collision.
         */
        helios::engine::ecs::EntityHandle source_;

    public:

        /**
         * @brief Constructs a SolidCollisionEvent.
         *
         * @param source Handle of the reporting entity.
         * @param collisionContext Context data describing the collision.
         */
        explicit SolidCollisionEvent(
            const helios::engine::ecs::EntityHandle source,
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
         * @brief Returns the source entity handle.
         *
         * @return Handle of the entity that reported the collision.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle source() const noexcept {
            return source_;
        }

    };

}
