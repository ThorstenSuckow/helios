/**
 * @file HealthBuilder.ixx
 * @brief Builder for health-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.HealthBuilder;

import helios.engine.builder.gameObject.builders.configs.HealthConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring health-related components.
     *
     * Provides methods to create config objects for health components
     * such as HealthComponent.
     */
    template<typename Entity>
    class HealthBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a HealthBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit HealthBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for HealthComponent.
         *
         * @return HealthConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::HealthConfig<Entity> health() const {
            return helios::engine::builder::gameObject::builders::configs::HealthConfig<Entity>{gameObject_};
        }

    };

}
