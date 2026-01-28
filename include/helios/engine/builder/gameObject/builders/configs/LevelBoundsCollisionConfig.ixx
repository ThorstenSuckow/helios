/**
 * @file LevelBoundsCollisionConfig.ixx
 * @brief Configuration for level boundary collision behavior.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.LevelBoundsCollisionConfig;

import helios.engine.ecs.GameObject;

import helios.engine.mechanics.bounds.components.LevelBoundsBehaviorComponent;
import helios.engine.modules.physics.collision.types;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for level boundary collision behavior.
     *
     * Provides methods for configuring how entities respond when
     * reaching or crossing level boundaries.
     */
    class LevelBoundsCollisionConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

        /**
         * @brief Validates LevelBoundsBehaviorComponent presence.
         *
         * @param isAvailable Whether the component should exist.
         */
        void ensureLevelBoundsBehaviorComponent(const bool isAvailable) const noexcept {

            auto* lbcc = gameObject_->get<helios::engine::mechanics::bounds::components::LevelBoundsBehaviorComponent>();

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
        explicit LevelBoundsCollisionConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Sets the collision behavior at level boundaries.
         *
         * @param collisionBehavior The behavior (clamp, bounce, wrap, despawn).
         *
         * @return Reference to this config for chaining.
         */
        LevelBoundsCollisionConfig& onCollision(helios::engine::modules::physics::collision::types::CollisionBehavior collisionBehavior) {

            gameObject_->getOrAdd<helios::engine::mechanics::bounds::components::LevelBoundsBehaviorComponent>()
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

            gameObject_->getOrAdd<helios::engine::mechanics::bounds::components::LevelBoundsBehaviorComponent>()
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
        LevelBoundsCollisionConfig& respondWith(helios::engine::modules::physics::collision::types::CollisionResponse collisionResponse) {

            gameObject_->getOrAdd<helios::engine::mechanics::bounds::components::LevelBoundsBehaviorComponent>()
                        .setCollisionResponse(collisionResponse);

            return *this;
        }


    };

}

