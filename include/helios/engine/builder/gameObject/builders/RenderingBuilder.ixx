/**
 * @file RenderingBuilder.ixx
 * @brief Builder for rendering-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.RenderingBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.RenderableConfig;
import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;

export namespace helios::engine::builder::gameObject::builders {

    /**
     * @brief Builder for configuring visual representation components.
     *
     * Provides factory methods for setting up meshes, materials,
     * shaders, and scene graph integration.
     */
    class RenderingBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

    public:

        /**
         * @brief Constructs a RenderingBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit RenderingBuilder(helios::engine::ecs::GameObject* gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for renderable (mesh/material) setup.
         *
         * @return RenderableConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::RenderableConfig renderable() const {
            return helios::engine::builder::gameObject::builders::configs::RenderableConfig{gameObject_};
        }

        /**
         * @brief Returns a config for scene node setup.
         *
         * @return SceneNodeConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::SceneNodeConfig sceneNode() const {
            return helios::engine::builder::gameObject::builders::configs::SceneNodeConfig{gameObject_};
        }

    };

}

