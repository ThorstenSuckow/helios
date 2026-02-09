/**
 * @file CollisionBuilder.ixx
 * @brief Builder for collision-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.CollisionBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.CollisionConfig;
import helios.engine.builder.gameObject.builders.configs.LevelBoundsCollisionConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring collision detection components.
     *
     * Provides factory methods for setting up AABB colliders,
     * collision layers, and level boundary behavior.
     */
    class CollisionBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject gameObject_;

    public:

        /**
         * @brief Constructs a CollisionBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit CollisionBuilder(helios::engine::ecs::GameObject gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for general collision setup.
         *
         * @return CollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::CollisionConfig collision() const {
            return helios::engine::builder::gameObject::builders::configs::CollisionConfig{gameObject_};
        }

        /**
         * @brief Returns a config for level bounds collision behavior.
         *
         * @return LevelBoundsCollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::engine::builder::gameObject::builders::configs::LevelBoundsCollisionConfig levelBoundsCollision() const {
            return helios::engine::builder::gameObject::builders::configs::LevelBoundsCollisionConfig{gameObject_};
        }

    };

}
