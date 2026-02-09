/**
 * @file WeaponConfig.ixx
 * @brief Configuration for weapon and combat components.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.WeaponConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.spatial.transform.components;

import helios.engine.mechanics.combat.components;

export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for weapon and projectile systems.
     *
     * Automatically adds Aim2DComponent and provides methods
     * for setting fire rate and other weapon parameters.
     */
    class WeaponConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
         helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a WeaponConfig and adds Aim2DComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit WeaponConfig(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {
            gameObject_.add<helios::engine::mechanics::combat::components::Aim2DComponent>();
        }

        /**
         * @brief Sets the weapon fire rate.
         *
         * @param fireRate Time in seconds between shots.
         *
         * @return Reference to this config for chaining.
         */
        WeaponConfig& fireRate(const float fireRate) {
            gameObject_.getOrAdd<helios::engine::mechanics::combat::components::ShootComponent>()
                        .setFireRate(fireRate);

            auto* transformComponent_ = gameObject_.get<helios::engine::modules::spatial::transform::components::ComposeTransformComponent>();
            auto* ac = gameObject_.get<helios::engine::mechanics::combat::components::Aim2DComponent>();
            assert(transformComponent_ != nullptr && "Unexpected nullptr for transformComponent_");
            assert(ac != nullptr && "Unexpected nullptr for Aim2DComponent");

            return *this;
        }

    };

}

