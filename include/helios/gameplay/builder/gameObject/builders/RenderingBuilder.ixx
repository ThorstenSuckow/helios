/**
 * @file RenderingBuilder.ixx
 * @brief Builder for rendering-related component configuration.
 */
module;

export module helios.gameplay.builder.gameObject.builders.RenderingBuilder;

import helios.gameplay.builder.gameObject.builders.configs.MeshRenderableConfig;
import helios.gameplay.builder.gameObject.builders.configs.TextRenderableConfig;
import helios.gameplay.builder.gameObject.builders.configs.SceneNodeConfig;

export namespace helios::gameplay::builder::gameObject::builders {

    /**
     * @brief Builder for configuring visual representation components.
     *
     * Provides factory methods for setting up meshes, materials,
     * shaders, and scene graph integration.
     */
    template<typename Entity>
    class RenderingBuilder {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

    public:

        /**
         * @brief Constructs a RenderingBuilder for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit RenderingBuilder(Entity gameObject) : gameObject_(gameObject) {}

        /**
         * @brief Returns a config for renderable (mesh/material) setup.
         *
         * @return MeshRenderableConfig for fluent configuration.
         */
        helios::gameplay::builder::gameObject::builders::configs::MeshRenderableConfig<Entity> meshRenderable() const {
            return helios::gameplay::builder::gameObject::builders::configs::MeshRenderableConfig<Entity>{gameObject_};
        }

        /**
         * @brief Returns a config for text renderable setup.
         *
         * @return TextRenderableConfig for fluent configuration.
         */
        helios::gameplay::builder::gameObject::builders::configs::TextRenderableConfig<Entity> textRenderable() const {
            return helios::gameplay::builder::gameObject::builders::configs::TextRenderableConfig<Entity>{gameObject_};
        }

        /**
         * @brief Returns a config for scene node setup.
         *
         * @return SceneNodeConfig for fluent configuration.
         */
        helios::gameplay::builder::gameObject::builders::configs::SceneNodeConfig<Entity> sceneNode() const {
            return helios::gameplay::builder::gameObject::builders::configs::SceneNodeConfig<Entity>{gameObject_};
        }

    };

}

