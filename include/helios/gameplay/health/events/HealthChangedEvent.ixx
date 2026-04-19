/**
 * @file HealthChangedEvent.ixx
 * @brief Event emitted when an entity's health changes.
 */
module;

#include <optional>

export module helios.gameplay.health.events.HealthChangedEvent;

import helios.util.Guid;
import helios.math;
import helios.gameplay.health.types.HealthChangeContext;
import helios.core.types;

import helios.ecs.types.EntityHandle;

export namespace helios::gameplay::health::events {

    /**
     * @brief Event carrying details of a health modification.
     *
     * Emitted by the health system whenever an entity's health value
     * changes, allowing observers to react (e.g. UI updates, VFX).
     */
    template<typename THandle>
    class HealthChangedEvent {

        /**
         * @brief Context describing the health change.
         */
        helios::gameplay::health::types::HealthChangeContext<THandle> healthChangeContext_;

    public:

        /**
         * @brief Constructs a HealthChangedEvent.
         *
         * @param HealthChangeContext Context describing the change.
         */
        explicit HealthChangedEvent(
                const helios::gameplay::health::types::HealthChangeContext<THandle>& HealthChangeContext
        ) : healthChangeContext_(HealthChangeContext) {}

        /**
         * @brief Returns the health update context.
         *
         * @return The HealthChangeContext for this event.
         */
        [[nodiscard]] helios::gameplay::health::types::HealthChangeContext<THandle> HealthChangeContext() const noexcept {
            return healthChangeContext_;
        }


    };

}
