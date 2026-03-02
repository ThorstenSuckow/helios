/**
 * @file HealthConfig.ixx
 * @brief Configuration for health components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.HealthConfig;

import helios.engine.ecs.GameObject;

import helios.engine.mechanics.health;
import helios.engine.mechanics.match.components;

using namespace helios::engine::mechanics::health::components;
using namespace helios::engine::mechanics::match::components;
using namespace helios::engine::mechanics::health::types;

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
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a HealthConfig and adds HealthComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit HealthConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {
            gameObject.add<HealthComponent>();
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The maximum health value.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& maxHealth(const float maxHealth) {
            gameObject_.getOrAdd<HealthComponent>()
                        .setMaxHealth(maxHealth);
            gameObject_.getOrAdd<HealthComponent>()
                       .heal(maxHealth);

            return *this;
        }

        /**
         * @brief Sets the initial number of lives.
         *
         * Adds or retrieves a LivesComponent with the given count.
         *
         * @param lives Initial life count.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& lives(const size_t lives) {
            gameObject_.getOrAdd<LivesComponent>(lives);

            return *this;
        }



        /**
         * @brief Configures the response when health reaches zero.
         *
         * @param behavior Bitmask of HealthDepletedBehavior flags.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& healthDepletedTriggers(const HealthDepletedBehavior behavior) {
            gameObject_.getOrAdd<HealthComponent>()
                        .setHealthDepletedBehavior(behavior);

            return *this;
        }

    };

}
