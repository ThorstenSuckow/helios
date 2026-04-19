/**
 * @file HealthBuilder.ixx
 * @brief Builder for health-related component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.HealthBuilder;

import helios.gameplay.builder.entity.builders.configs.HealthConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring health-related components.
     *
     * Provides methods to create config objects for health components
     * such as HealthComponent.
     */
    template<typename Entity>
    class HealthBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a HealthBuilder for the given Entity.
         *
         * @param entity The target Entity to configure.
         */
        explicit HealthBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config object for HealthComponent.
         *
         * @return HealthConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::HealthConfig<Entity> health() const {
            return helios::gameplay::builder::entity::builders::configs::HealthConfig<Entity>{entity_};
        }

    };

}
