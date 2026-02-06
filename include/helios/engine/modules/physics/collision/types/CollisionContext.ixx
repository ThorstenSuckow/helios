/**
 * @file CollisionContext.ixx
 * @brief Context data for collision events.
 */
module;

#include <optional>

export module helios.engine.modules.physics.collision.types.CollisionContext;

import helios.math;

import helios.util;

import helios.engine.ecs.EntityHandle;
import helios.core.types;

export namespace helios::engine::modules::physics::collision::types {

    /**
     * @brief Context data describing a collision event.
     *
     * Contains all relevant information about a collision between two entities,
     * including the entities involved, contact point, and collision properties.
     */
    struct CollisionContext {

        /**
         * @brief Handle of the source entity that initiated the collision check.
         */
        helios::engine::ecs::EntityHandle source;

        /**
         * @brief World-space contact point of the collision.
         */
        helios::math::vec3f contact;

        /**
         * @brief True if the collision involves a solid (blocking) entity.
         */
        bool isSolid;

        /**
         * @brief True if this is a trigger collision (non-blocking).
         */
        bool isTrigger;

        /**
         * @brief True if the source entity reports collision events.
         */
        bool isCollisionReporter;

        /**
         * @brief Handle of the other entity involved in the collision.
         *
         * May be empty if the collision is with a non-entity object (e.g., world bounds).
         */
        std::optional<helios::engine::ecs::EntityHandle> other;

        /**
         * @brief Collision layer ID of the source entity.
         */
        uint32_t collisionLayerId;

        /**
         * @brief Collision layer ID of the other entity.
         */
        uint32_t otherCollisionLayerId;

    };

}