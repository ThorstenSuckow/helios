/**
 * @file ShootingBuilder.ixx
 * @brief Builder for weapon and combat component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.ShootingBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.WeaponConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring weapon and projectile components.
     *
     * Provides factory methods for setting up weapon systems
     * including fire rate and projectile spawning.
     */
    class ShootingBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a ShootingBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit ShootingBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for weapon setup.
         *
         * @return WeaponConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::WeaponConfig weapon() const {
            return helios::engine::builder::gameObject::builders::configs::WeaponConfig{gameObject_};
        }

    };

}
