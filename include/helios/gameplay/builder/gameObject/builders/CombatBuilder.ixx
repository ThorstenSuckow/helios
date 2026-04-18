/**
 * @file CombatBuilder.ixx
 * @brief Builder for weapon and combat component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.CombatBuilder;

import helios.gameplay.builder.gameObject.builders.configs.WeaponConfig;
import helios.gameplay.builder.gameObject.builders.configs.CombatConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring weapon and projectile components.
     *
     * Provides factory methods for setting up weapon systems
     * including fire rate and projectile spawning.
     */
    template<typename Entity>
    class CombatBuilder {

        Entity gameObject_;

    public:

        /**
         * @brief Constructs a CombatBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit CombatBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for weapon setup.
         *
         * @return WeaponConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::WeaponConfig<Entity> weapon() const {
            return helios::gameplay::builder::gameObject::builders::configs::WeaponConfig<Entity>{gameObject_};
        }

        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::CombatConfig<Entity> combat() const {
            return helios::gameplay::builder::gameObject::builders::configs::CombatConfig<Entity>{gameObject_};
        }

    };

}
