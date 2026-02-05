/**
 * @file HealthBuilder.ixx
 * @brief Builder for health-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.HealthBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.HealthConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring health-related components.
     *
     * Provides methods to create config objects for health components
     * such as HealthComponent.
     */
    class HealthBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a HealthBuilder for the given GameObject.
         *
         * @param gameObject The target GameObject to configure.
         */
        explicit HealthBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config object for HealthComponent.
         *
         * @return HealthConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::HealthConfig health() const {
            return helios::engine::builder::gameObject::builders::configs::HealthConfig{gameObject_};
        }

    };

}
