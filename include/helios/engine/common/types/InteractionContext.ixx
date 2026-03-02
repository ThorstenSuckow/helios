/**
 * @file InteractionContext.ixx
 * @brief Context struct describing a generic entity-to-entity interaction.
 */

export module helios.engine.common.types.InteractionContext;

import helios.math;

import helios.util;

import helios.core.types;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::common::types {

    /**
     * @brief Describes a generic interaction between entities.
     *
     * Carries the participating entity handles and the world-space contact
     * point. Used as a base context for more specific interaction types
     * such as DamageContext.
     */
    struct InteractionContext {

        /**
         * @brief Entity that is affected by the interaction.
         */
        helios::engine::ecs::EntityHandle target{};

        /**
         * @brief Entity that initiated the interaction.
         */
        helios::engine::ecs::EntityHandle instigator{};

        /**
         * @brief Entity that directly caused the interaction (e.g. projectile).
         */
        helios::engine::ecs::EntityHandle causer{};

        /**
         * @brief World-space contact point of the interaction.
         */
        helios::math::vec3f contact{};

    };

}