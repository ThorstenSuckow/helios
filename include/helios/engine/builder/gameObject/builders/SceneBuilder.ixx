/**
 * @file SceneBuilder.ixx
 * @brief Builder for scene graph component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.SceneBuilder;

import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring scene graph integration.
     *
     * Provides factory methods for setting up scene node
     * parenting and transform inheritance.
     */
    template<typename Entity>
    class SceneBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a SceneBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit SceneBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for scene node setup.
         *
         * @return SceneNodeConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::SceneNodeConfig<Entity> sceneNode() const {
            return helios::engine::builder::gameObject::builders::configs::SceneNodeConfig<Entity>{gameObject_};
        }

    };

}
