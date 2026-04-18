/**
 * @file HealthConfig.ixx
 * @brief Configuration for health components.
 */
module;

export module helios.engine.builder.gameObject.builders.configs.HealthConfig;


import helios.gameplay.health;
import helios.gameplay.match.components;

using namespace helios::gameplay::health::components;
using namespace helios::gameplay::match::components;
using namespace helios::gameplay::health::types;

export namespace helios::engine::builder::gameObject::builders::configs {


    /**
     * @brief Fluent configuration for HealthComponent setup.
     *
     * Automatically adds HealthComponent and provides methods for
     * setting maximum health.
     */
    template<typename Entity>
    class HealthConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a HealthConfig and adds HealthComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit HealthConfig(Entity gameObject) : gameObject_(gameObject) {
            gameObject.template add<HealthComponent<Handle_type>>();
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The maximum health value.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& maxHealth(const float maxHealth) {
            gameObject_.template getOrAdd<HealthComponent<Handle_type>>()
                        .setMaxHealth(maxHealth);
            gameObject_.template getOrAdd<HealthComponent<Handle_type>>()
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
            gameObject_.template getOrAdd<LivesComponent<Handle_type>>(lives);

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
            gameObject_.template getOrAdd<HealthComponent<Handle_type>>()
                        .setHealthDepletedBehavior(behavior);

            return *this;
        }

    };

}
