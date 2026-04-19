/**
 * @file SceneBuilder.ixx
 * @brief Builder for scene graph component configuration.
 */
module;

export module helios.gameplay.builder.entity.builders.SceneBuilder;

import helios.gameplay.builder.entity.builders.configs.SceneNodeConfig;

export namespace helios::gameplay::builder::entity::builders {

    /**
     * @brief Builder for configuring scene graph integration.
     *
     * Provides factory methods for setting up scene node
     * parenting and transform inheritance.
     */
    template<typename Entity>
    class SceneBuilder {

        /**
         * @brief Non-owning pointer to the target Entity.
         */
        Entity entity_;

    public:

        /**
         * @brief Constructs a SceneBuilder for the given Entity.
         *
         * @param entity Target Entity to configure.
         */
        explicit SceneBuilder(Entity entity) : entity_(entity) {}

        /**
         * @brief Returns a config for scene node setup.
         *
         * @return SceneNodeConfig for fluent configuration.
         */
        helios::gameplay::builder::entity::builders::configs::SceneNodeConfig<Entity> sceneNode() const {
            return helios::gameplay::builder::entity::builders::configs::SceneNodeConfig<Entity>{entity_};
        }

    };

}
