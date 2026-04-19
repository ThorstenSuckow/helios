/**
 * @file WeaponConfig.ixx
 * @brief Configuration for weapon and combat components.
 */
module;

#include <cassert>
#include <memory>

export module helios.gameplay.builder.entity.builders.configs.WeaponConfig;


import helios.spatial.transform.components;

import helios.gameplay.combat.components;

export namespace helios::gameplay::builder::entity::builders::configs {

    /**
     * @brief Fluent configuration for weapon and projectile systems.
     *
     * Automatically adds Aim2DComponent and provides methods
     * for setting fire rate and other weapon parameters.
     */
    template<typename Entity>
    class WeaponConfig {

        using Handle_type = typename Entity::Handle_type;

         Entity entity_;

    public:

        /**
         * @brief Constructs a WeaponConfig and adds Aim2DComponent.
         *
         * @param entity Target Entity to configure.
         */
        explicit WeaponConfig(Entity entity) : entity_(entity) {
            entity_.template add<helios::gameplay::combat::components::Aim2DComponent<Handle_type>>();
        }

        /**
         * @brief Sets the weapon fire rate.
         *
         * @param fireRate Time in seconds between shots.
         *
         * @return Reference to this config for chaining.
         */
        WeaponConfig& fireRate(const float fireRate) {
            entity_.template getOrAdd<helios::gameplay::combat::components::ShootComponent<Handle_type>>()
                        .setFireRate(fireRate);

            auto* transformComponent_ = entity_.get<helios::spatial::transform::components::ComposeTransformComponent<Handle_type>>();
            auto* ac = entity_.get<helios::gameplay::combat::components::Aim2DComponent<Handle_type>>();
            assert(transformComponent_ != nullptr && "Unexpected nullptr for transformComponent_");
            assert(ac != nullptr && "Unexpected nullptr for Aim2DComponent");

            return *this;
        }

    };

}

