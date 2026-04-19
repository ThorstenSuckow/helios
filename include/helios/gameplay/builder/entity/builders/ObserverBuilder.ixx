/**
 * @file ObserverBuilder.ixx
 * @brief Builder for observer component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.ObserverBuilder;

import helios.gameplay.builder.entity.builders.configs.ObserverConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring observer components.
     *
     * Provides methods to create config objects for observer components
     * that bind to data sources like score pools.
     */
    template<typename Entity>
    class ObserverBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs an ObserverBuilder for the given Entity.
         *
         * @param entity The target Entity to configure.
         */
        explicit ObserverBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config object for observer components.
         *
         * @return ObserverConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::ObserverConfig<Entity> observe() const {
            return helios::gameplay::builder::entity::builders::configs::ObserverConfig<Entity>{entity_};
        }

    };

}
