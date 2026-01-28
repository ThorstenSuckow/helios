/**
 * @file CollisionConfig.ixx
 * @brief Configuration for collision detection components.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.builder.gameObject.builders.configs.CollisionConfig;

import helios.engine.ecs.GameObject;

import helios.engine.modules.physics.collision.components;
import helios.engine.modules.physics.collision.types;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for collision detection setup.
     *
     * Automatically adds CollisionStateComponent and provides methods
     * for configuring collision layers, masks, and behaviors.
     */
    class CollisionConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

        /**
         * @brief Validates CollisionComponent presence.
         *
         * @param isAvailable Whether the component should exist.
         */
        void ensureCollisionComponent(const bool isAvailable) const noexcept {

            auto* cc = gameObject_->get<helios::engine::modules::physics::collision::components::CollisionComponent>();

            if (isAvailable) {
                assert(cc && "CollisionComponent not configured");
            } else {
                assert(!cc && "CollisionComponent already configured");
            }

        }

    public:

        /**
         * @brief Constructs a CollisionConfig and adds CollisionStateComponent.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit CollisionConfig(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {

            gameObject_->add<helios::engine::modules::physics::collision::components::CollisionStateComponent>();

        }

        /**
         * @brief Adds AABB collider for bounding box collision.
         *
         * @return Reference to this config for chaining.
         */
        CollisionConfig& useBoundingBox() {
            gameObject_->add<helios::engine::modules::physics::collision::components::AabbColliderComponent>();
            return *this;
        }

        /**
         * @brief Sets the collision layer ID.
         *
         * @param layerId The layer this entity belongs to.
         *
         * @return Reference to this config for chaining.
         */
        CollisionConfig& layerId(const uint32_t layerId) {

            ensureCollisionComponent(false);

            gameObject_->add<helios::engine::modules::physics::collision::components::CollisionComponent>(layerId);

            return *this;
        }

        /**
         * @brief Sets the hit policy for collision detection.
         *
         * @details Controls how many collision events this entity receives per frame:
         * - `OneHit`: Only the first collision is reported (default)
         * - `All`: All collisions with overlapping entities are reported
         *
         * @param hitPolicy The hit policy to apply.
         *
         * @return Reference to this config for chaining.
         *
         * @see HitPolicy
         */
        CollisionConfig& hitPolicy(const helios::engine::modules::physics::collision::types::HitPolicy hitPolicy) {

            ensureCollisionComponent(true);

            gameObject_->get<helios::engine::modules::physics::collision::components::CollisionComponent>()
                        ->setHitPolicy(hitPolicy);

            return *this;
        }

        /**
         * @brief Sets whether this entity reports collisions.
         *
         * @param isCollisionReporter True to generate collision events.
         *
         * @return Reference to this config for chaining.
         */
        CollisionConfig& reportCollisions(bool isCollisionReporter) {

            ensureCollisionComponent(true);

            gameObject_->get<helios::engine::modules::physics::collision::components::CollisionComponent>()
                       ->setIsCollisionReporter(isCollisionReporter);
            return *this;
        }

        /**
         * @brief Sets the solid collision mask.
         *
         * @param solidCollisionMask Bitmask of layers to collide with.
         *
         * @return Reference to this config for chaining.
         */
        CollisionConfig& solidCollisionMask(const uint32_t solidCollisionMask) {
            ensureCollisionComponent(true);

            gameObject_->get<helios::engine::modules::physics::collision::components::CollisionComponent>()
                      ->setSolidCollisionMask(solidCollisionMask);
            return *this;
        }

        /**
         * @brief Adds a collision behavior for a specific layer.
         *
         * @param otherLayerId The layer to respond to.
         * @param collisionBehavior The behavior when colliding.
         *
         * @return Reference to this config for chaining.
         */
        CollisionConfig& onSolidCollision(
            const uint32_t otherLayerId,
            const helios::engine::modules::physics::collision::types::CollisionBehavior collisionBehavior
        ) {
            ensureCollisionComponent(true);

            gameObject_->get<helios::engine::modules::physics::collision::components::CollisionComponent>()
                      ->addSolidCollisionBehavior(collisionBehavior, otherLayerId);

            return *this;
        }


    };

}

