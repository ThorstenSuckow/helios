/**
 * @file LevelBoundsCollisionConfig.ixx
 * @brief Configuration for level boundary collision behavior.
 */
module;

#include <cassert>
#include <memory>

export module helios.gameplay.builder.gameObject.builders.configs.LevelBoundsCollisionConfig;


import helios.gameplay.bounds.components.LevelBoundsBehaviorComponent;
import helios.physics.collision.types;


export namespace helios::gameplay::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for level boundary collision behavior.
     *
     * Provides methods for configuring how entities respond when
     * reaching or crossing level boundaries.
     */
    template<typename Entity>
    class LevelBoundsCollisionConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

        /**
         * @brief Validates LevelBoundsBehaviorComponent presence.
         *
         * @param isAvailable Whether the component should exist.
         */
        void ensureLevelBoundsBehaviorComponent(const bool isAvailable) const noexcept {

            auto* lbcc = gameObject_.template get<helios::gameplay::bounds::components::LevelBoundsBehaviorComponent<Handle_type>>();

            if (isAvailable) {
                assert(lbcc && "LevelBoundsBehaviorComponent not configured");
            } else {
                assert(!lbcc && "LevelBoundsBehaviorComponent already configured");
            }

        }

    public:

        /**
         * @brief Constructs a LevelBoundsCollisionConfig.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit LevelBoundsCollisionConfig(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Sets the collision behavior at level boundaries.
         *
         * @param collisionBehavior The behavior (clamp, bounce, wrap, despawn).
         *
         * @return Reference to this config for chaining.
         */
        LevelBoundsCollisionConfig& onCollision(helios::physics::collision::types::CollisionBehavior collisionBehavior) {

            gameObject_.template getOrAdd<helios::gameplay::bounds::components::LevelBoundsBehaviorComponent<Handle_type>>()
                        .setCollisionBehavior(collisionBehavior);

            return *this;
        }

        /**
         * @brief Sets the restitution coefficient for bounce behavior.
         *
         * @param restitution Bounce factor (1.0 = perfect bounce).
         *
         * @return Reference to this config for chaining.
         */
        LevelBoundsCollisionConfig& restitution(const float restitution) {

            gameObject_.template getOrAdd<helios::gameplay::bounds::components::LevelBoundsBehaviorComponent<Handle_type>>()
                       .setRestitution(restitution);

            return *this;
        }

        /**
         * @brief Sets the collision response type.
         *
         * @param collisionResponse How to respond (slide, stop, etc.).
         *
         * @return Reference to this config for chaining.
         */
        LevelBoundsCollisionConfig& respondWith(helios::physics::collision::types::CollisionResponse collisionResponse) {

            gameObject_.template getOrAdd<helios::gameplay::bounds::components::LevelBoundsBehaviorComponent<Handle_type>>()
                        .setCollisionResponse(collisionResponse);

            return *this;
        }


    };

}

