/**
 * @file HealthDepletedEvent.ixx
 * @brief Event emitted when an entity dies.
 */
module;

#include <optional>

export module helios.engine.mechanics.health.events.HealthDepletedEvent;

import helios.util.Guid;
import helios.math;
import helios.engine.common.types.DamageContext;
import helios.core.types;

import helios.engine.ecs.EntityHandle;

using namespace helios::engine::common::types;

export namespace helios::engine::mechanics::health::events {

    /**
     * @brief Event emitted when an entity's health reaches zero.
     *
     * Contains the handle of the deceased entity and optionally the
     * attack context if the death was caused by combat damage.
     */
    class HealthDepletedEvent {

        /**
         * @brief Handle of the entity that died.
         */
        helios::engine::ecs::EntityHandle source_;

        /**
         * @brief Attack context if death was caused by combat.
         */
        std::optional<DamageContext> damageContext_;

    public:


        /**
         * @brief Constructs a HealthDepletedEvent.
         *
         * @param source Handle of the deceased entity.
         * @param damageContext Optional damage context if death was from damage applied.
         */
        explicit HealthDepletedEvent(
            const helios::engine::ecs::EntityHandle source,
            std::optional<DamageContext> damageContext = std::nullopt
        ) : damageContext_(damageContext), source_(source) {}

        /**
         * @brief Returns the damage context if available.
         *
         * @return Optional DamageContext, or nullopt if death was not from combat.
         */
        [[nodiscard]] std::optional<DamageContext> damageContext() const noexcept {
            return damageContext_;
        }

        /**
         * @brief Returns the handle of the deceased entity.
         *
         * @return The source entity handle.
         */
        [[nodiscard]] helios::engine::ecs::EntityHandle source() const noexcept {
            return source_;
        }

    };

}
