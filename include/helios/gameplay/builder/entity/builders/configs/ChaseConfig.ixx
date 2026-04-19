/**
 * @file ChaseConfig.ixx
 * @brief Configuration for AI chase behavior.
 */
module;

#include <cassert>

export module helios.gameplay.builder.entity.builders.configs.ChaseConfig;


import helios.util.Guid;

import helios.gameplay.ai.components.ChaseComponent;


export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for AI chase behavior.
     *
     * Automatically adds ChaseComponent and provides methods
     * for setting the chase target and seek interval.
     *
     * @tparam Entity The entity type (e.g., Entity).
     */
    template<typename Entity>
    class ChaseConfig {

        /**
         * @brief Non-owning pointer to the target entity.
         */
        Entity entity_;

    public:

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Constructs a ChaseConfig and adds ChaseComponent.
         *
         * @param entity Target entity to configure.
         */
        explicit ChaseConfig(Entity entity) : entity_(entity) {
            entity.template add<helios::gameplay::ai::components::ChaseComponent<Handle_type>>();
        }

        /**
         * @brief Sets the target entity to chase.
         *
         * @param entityHandle The handle of the target Entity.
         *
         * @return Reference to this config for chaining.
         */
        ChaseConfig& target(const Handle_type& entityHandle) {
            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");
            entity_.template getOrAdd<helios::gameplay::ai::components::ChaseComponent<Handle_type>>()
                        .setTarget(entityHandle);

            return *this;
        }

        /**
         * @brief Sets the interval between target position updates.
         *
         * @param interval Time in seconds between seek updates.
         *
         * @return Reference to this config for chaining.
         */
        ChaseConfig& seekInterval(const float interval) {
            entity_.template getOrAdd<helios::gameplay::ai::components::ChaseComponent<Handle_type>>()
                        .setCooldown(interval);

            return *this;
        }

    };

}

