/**
 * @file RenderingBuilder.ixx
 * @brief Builder for rendering-related component configuration.
 */
module;

export module helios.engine.builder.gameObject.builders.RenderingBuilder;

import helios.engine.ecs.GameObject;
import helios.engine.builder.gameObject.builders.configs.MeshRenderableConfig;
import helios.engine.builder.gameObject.builders.configs.TextRenderableConfig;
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
         * @return MeshRenderableConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::MeshRenderableConfig meshRenderable() const {
            return helios::engine::builder::gameObject::builders::configs::MeshRenderableConfig{gameObject_};
        }

        /**
         * @brief Returns a config for text renderable setup.
         *
         * @return TextRenderableConfig for fluent configuration.
         */
        helios::engine::builder::gameObject::builders::configs::TextRenderableConfig textRenderable() const {
            return helios::engine::builder::gameObject::builders::configs::TextRenderableConfig{gameObject_};
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

