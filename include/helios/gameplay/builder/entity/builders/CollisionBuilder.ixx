/**
 * @file CollisionBuilder.ixx
 * @brief Builder for collision-related component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.CollisionBuilder;

import helios.gameplay.builder.entity.builders.configs.CollisionConfig;
import helios.gameplay.builder.entity.builders.configs.LevelBoundsCollisionConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring collision detection components.
     *
     * Provides factory methods for setting up AABB colliders,
     * collision layers, and level boundary behavior.
     */
    template<typename Entity>
    class CollisionBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a CollisionBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit CollisionBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for general collision setup.
         *
         * @return CollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::CollisionConfig<Entity> collision() const {
            return helios::gameplay::builder::entity::builders::configs::CollisionConfig<Entity>{entity_};
        }

        /**
         * @brief Returns a config for level bounds collision behavior.
         *
         * @return LevelBoundsCollisionConfig for fluent configuration.
         */
        [[nodiscard]] helios::gameplay::builder::entity::builders::configs::LevelBoundsCollisionConfig<Entity> levelBoundsCollision() const {
            return helios::gameplay::builder::entity::builders::configs::LevelBoundsCollisionConfig<Entity>{entity_};
        }

    };

}
