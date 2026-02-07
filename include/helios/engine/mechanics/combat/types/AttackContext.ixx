/**
 * @file AttackContext.ixx
 * @brief Data structure containing attack information.
 */
module;


export module helios.engine.mechanics.combat.types.AttackContext;

import helios.math;

import helios.util;

import helios.core.types;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::mechanics::combat::types {

    /**
     * @brief Data structure containing information about an attack.
     *
     * Stores the attacker's identity, contact point, and damage dealt.
     * Used by LastAttackerComponent and DeathEvent for kill attribution.
     */
    struct AttackContext {

        /**
         * @brief Handle of the entity that performed the attack.
         */
        helios::engine::ecs::EntityHandle source{};

        /**
         * @brief World-space contact point where the attack landed.
         */
        helios::math::vec3f contact{};

        /**
         * @brief Net damage applied after any modifiers.
         */
        float damageApplied{};


    };

}