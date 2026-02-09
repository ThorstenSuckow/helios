/**
 * @file CombatBuilder.ixx
 * @brief Builder for weapon and combat component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.CombatBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.WeaponConfig;
import helios.engine.builder.gameObject.builders.configs.CombatConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring weapon and projectile components.
     *
     * Provides factory methods for setting up weapon systems
     * including fire rate and projectile spawning.
     */
    class CombatBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a CombatBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit CombatBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for weapon setup.
         *
         * @return WeaponConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::WeaponConfig weapon() const {
            return helios::engine::builder::gameObject::builders::configs::WeaponConfig{gameObject_};
        }

        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::CombatConfig combat() const {
            return helios::engine::builder::gameObject::builders::configs::CombatConfig{gameObject_};
        }

    };

}
