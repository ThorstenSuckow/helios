/**
 * @file HealthConfig.ixx
 * @brief Configuration for health components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.HealthConfig;

import helios.engine.ecs.GameObject;

import helios.engine.mechanics.health.components.HealthComponent;

export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for HealthComponent setup.
     *
     * Automatically adds HealthComponent and provides methods for
     * setting maximum health.
     */
    class HealthConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a HealthConfig and adds HealthComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit HealthConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {
            gameObject_->add<helios::engine::mechanics::health::components::HealthComponent>();
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The maximum health value.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& maxHealth(const float maxHealth) {
            gameObject_->getOrAdd<helios::engine::mechanics::health::components::HealthComponent>()
                        .setMaxHealth(maxHealth);

            return *this;
        }

    };

}
