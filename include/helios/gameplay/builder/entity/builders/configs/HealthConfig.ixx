/**
 * @file HealthConfig.ixx
 * @brief Configuration for health components.
 */
module;


#include <cstddef>
export module helios.gameplay.builder.entity.builders.configs.HealthConfig;


import helios.gameplay.health;
import helios.gameplay.matchstate.components;

using namespace helios::gameplay::health::components;
using namespace helios::gameplay::matchstate::components;
using namespace helios::gameplay::health::types;

export namespace helios::gameplay::builder::entity::builders::configs {


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
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a HealthConfig and adds HealthComponent.
         *
         * @param entity Target Entity to configure.
         */
        explicit HealthConfig(Entity entity) : entity_(entity) {
            entity.template add<HealthComponent<Handle_type>>();
        }

        /**
         * @brief Sets the maximum health value.
         *
         * @param maxHealth The maximum health value.
         *
         * @return Reference to this config for chaining.
         */
        HealthConfig& maxHealth(const float maxHealth) {
            entity_.template getOrAdd<HealthComponent<Handle_type>>()
                        .setMaxHealth(maxHealth);
            entity_.template getOrAdd<HealthComponent<Handle_type>>()
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
            entity_.template getOrAdd<LivesComponent<Handle_type>>(lives);

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
            entity_.template getOrAdd<HealthComponent<Handle_type>>()
                        .setHealthDepletedBehavior(behavior);

            return *this;
        }

    };

}
