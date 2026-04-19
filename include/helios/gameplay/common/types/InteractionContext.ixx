/**
 * @file InteractionContext.ixx
 * @brief Context struct describing a generic entity-to-entity interaction.
 */

export module helios.gameplay.common.types.InteractionContext;

import helios.math;

import helios.util;

import helios.core.types;

import helios.ecs.types.EntityHandle;

export namespace helios::gameplay::common::types {

    /**
     * @brief Describes a generic interaction between entities.
     *
     * Carries the participating entity handles and the world-space contact
     * point. Used as a base context for more specific interaction types
     * such as DamageContext.
     */
    template<typename THandle>
    struct InteractionContext {

        /**
         * @brief Entity that is affected by the interaction.
         */
        THandle target{};

        /**
         * @brief Entity that initiated the interaction.
         */
        THandle instigator{};

        /**
         * @brief Entity that directly caused the interaction (e.g. projectile).
         */
        THandle causer{};

        /**
         * @brief World-space contact point of the interaction.
         */
        helios::math::vec3f contact{};

    };

}