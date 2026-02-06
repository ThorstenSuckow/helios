/**
 * @file DeathEvent.ixx
 * @brief Event emitted when an entity dies.
 */
module;

#include <optional>

export module helios.engine.mechanics.health.events.DeathEvent;

import helios.util.Guid;
import helios.math;
import helios.engine.mechanics.combat.types.AttackContext;
import helios.core.types;

import helios.engine.ecs.EntityHandle;

export namespace helios::engine::mechanics::health::events {

    /**
     * @brief Event emitted when an entity's health reaches zero.
     *
     * Contains the handle of the deceased entity and optionally the
     * attack context if the death was caused by combat damage.
     */
    class DeathEvent {

        /**
         * @brief Handle of the entity that died.
         */
        helios::engine::ecs::EntityHandle source_;

        /**
         * @brief Attack context if death was caused by combat.
         */
        std::optional<helios::engine::mechanics::combat::types::AttackContext> attackContext_;

    public:


        /**
         * @brief Constructs a DeathEvent.
         *
         * @param source Handle of the deceased entity.
         * @param attackContext Optional attack context if death was from combat.
         */
        explicit DeathEvent(
            const helios::engine::ecs::EntityHandle source,
            std::optional<helios::engine::mechanics::combat::types::AttackContext> attackContext = std::nullopt
        ) : attackContext_(attackContext), source_(source) {}

        /**
         * @brief Returns the attack context if available.
         *
         * @return Optional AttackContext, or nullopt if death was not from combat.
         */
        [[nodiscard]] const std::optional<helios::engine::mechanics::combat::types::AttackContext> attackContext() const noexcept {
            return attackContext_;
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
