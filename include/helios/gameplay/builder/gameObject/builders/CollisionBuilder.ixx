/**
 * @file CollisionBuilder.ixx
 * @brief Builder for collision-related component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.CollisionBuilder;

import helios.gameplay.builder.gameObject.builders.configs.CollisionConfig;
import helios.gameplay.builder.gameObject.builders.configs.LevelBoundsCollisionConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring collision detection components.
     *
     * Provides factory methods for setting up AABB colliders,
     * collision layers, and level boundary behavior.
     */
    template<typename Entity>
    class CollisionBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a CollisionBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit CollisionBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for general collision setup.
         *
         * @return CollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::CollisionConfig<Entity> collision() const {
            return helios::gameplay::builder::gameObject::builders::configs::CollisionConfig<Entity>{gameObject_};
        }

        /**
         * @brief Returns a config for level bounds collision behavior.
         *
         * @return LevelBoundsCollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::gameObject::builders::configs::LevelBoundsCollisionConfig<Entity> levelBoundsCollision() const {
            return helios::gameplay::builder::gameObject::builders::configs::LevelBoundsCollisionConfig<Entity>{gameObject_};
        }

    };

}
