/**
 * @file HealthDepletedEvent.ixx
 * @brief Event emitted when an entity dies.
 */
module;

#include <optional>

export module helios.gameplay.health.events.HealthDepletedEvent;

import helios.util.Guid;
import helios.math;
import helios.gameplay.common.types.DamageContext;
import helios.core.types;

import helios.ecs.types.EntityHandle;

using namespace helios::gameplay::common::types;

export namespace helios::gameplay::health::events {

    /**
     * @brief Event emitted when an entity's health reaches zero.
     *
     * Contains the handle of the deceased entity and optionally the
     * attack context if the death was caused by combat damage.
     */
    template<typename THandle>
    class HealthDepletedEvent {

        /**
         * @brief Handle of the entity that died.
         */
        THandle source_;

        /**
         * @brief Attack context if death was caused by combat.
         */
        std::optional<DamageContext<THandle>> damageContext_;

    public:


        /**
         * @brief Constructs a HealthDepletedEvent.
         *
         * @param source Handle of the deceased entity.
         * @param damageContext Optional damage context if death was from damage applied.
         */
        explicit HealthDepletedEvent(
            const THandle source,
            std::optional<DamageContext<THandle>> damageContext = std::nullopt
        ) : damageContext_(damageContext), source_(source) {}

        /**
         * @brief Returns the damage context if available.
         *
         * @return Optional DamageContext, or nullopt if death was not from combat.
         */
        [[nodiscard]] std::optional<DamageContext<THandle>> damageContext() const noexcept {
            return damageContext_;
        }

        /**
         * @brief Returns the handle of the deceased entity.
         *
         * @return The source entity handle.
         */
        [[nodiscard]] THandle source() const noexcept {
            return source_;
        }

    };

}
