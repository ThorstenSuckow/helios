/**
 * @file HealthChangeContext.ixx
 * @brief Context data describing a health change applied to an entity.
 */

export module helios.engine.mechanics.health.types.HealthChangeContext;


import helios.engine.common.types.InteractionContext;

using namespace helios::engine::common::types;

export namespace helios::engine::mechanics::health::types {

    /**
     * @brief Describes a health modification applied to an entity.
     *
     * Combines the interaction context (target, instigator, causer, contact
     * point) with a signed delta value. A negative delta represents damage,
     * a positive delta represents healing.
     *
     * @see InteractionContext
     * @see HealthManager
     */
    struct HealthChangeContext {

        /**
         * @brief The interaction that caused the health change.
         */
        InteractionContext interactionContext{};

        /**
         * @brief Signed health delta (negative = damage, positive = healing).
         */
        float delta{};

    };

}