/**
 * @file DamageContext.ixx
 * @brief Context struct describing a damage interaction between entities.
 */

export module helios.engine.common.types.DamageContext;

import helios.engine.common.types.InteractionContext;


export namespace helios::engine::common::types {

    /**
     * @brief Extends InteractionContext with a damage amount.
     *
     * Carries all information needed to apply damage to a target entity:
     * the participating handles, the contact point (from InteractionContext),
     * and the amount of damage to apply.
     *
     * @see InteractionContext
     */
    struct DamageContext {

        /**
         * @brief The underlying interaction between source and target.
         */
        InteractionContext interactionContext{};

        /**
         * @brief Amount of damage to apply.
         */
        float damage{};

    };

}