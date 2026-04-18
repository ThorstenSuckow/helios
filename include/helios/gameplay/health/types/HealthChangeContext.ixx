/**
 * @file HealthChangeContext.ixx
 * @brief Context data describing a health change applied to an entity.
 */

export module helios.gameplay.health.types.HealthChangeContext;


import helios.engine.common.types.InteractionContext;

using namespace helios::engine::common::types;

export namespace helios::gameplay::health::types {

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
    template<typename THandle>
    struct HealthChangeContext {

        /**
         * @brief The interaction that caused the health change.
         */
        InteractionContext<THandle> interactionContext{};

        /**
         * @brief Signed health delta (negative = damage, positive = healing).
         */
        float delta{};

    };

}