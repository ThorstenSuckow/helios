/**
 * @file RenderableConfig.ixx
 * @brief Configuration for renderable components (mesh, material, shader).
 */
module;

#include <memory>
#include <unordered_map>
#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.RenderableConfig;

import helios.engine.ecs.GameObject;
import helios.rendering;
import helios.ext.opengl;
import helios.engine.modules.rendering;
import helios.math.types;
import helios.rendering.model.config;
import helios.scene.SceneNode;

import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for setting up a Renderable on a GameObject.
     *
     * Allows specification of shape, shader, primitive type, and color.
     * Builds and attaches a RenderableComponent.
     */
    class RenderableConfig {

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        helios::engine::ecs::GameObject* gameObject_;

        /**
         * @brief The shape geometry to render.
         */
        std::shared_ptr<helios::rendering::asset::shape::Shape> shape_;

        /**
         * @brief The shader for rendering.
         */
        std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> shader_;

        /**
         * @brief The primitive type for mesh rendering.
         */
        helios::rendering::model::config::PrimitiveType primitiveType_;

        /**
         * @brief The base color for the material.
         */
        helios::math::vec4f color_;

        static inline std::unordered_map<
            helios::rendering::model::config::PrimitiveType,
            std::shared_ptr<helios::rendering::model::config::MeshConfig>
        > meshConfigs_;

        static std::shared_ptr<helios::rendering::model::config::MeshConfig> meshConfig(helios::rendering::model::config::PrimitiveType primitiveType) {

            auto [it, inserted] = meshConfigs_.try_emplace(
                primitiveType,
                std::make_shared<helios::rendering::model::config::MeshConfig>(primitiveType)
            );

            return it->second;
        }

    public:

        /**
         * @brief Constructs a RenderableConfig for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit RenderableConfig(
            helios::engine::ecs::GameObject* gameObject
        ) : gameObject_(gameObject) {}

        /**
         * @brief Sets the shape geometry.
         *
         * @param shape Shared pointer to the shape.
         *
         * @return Reference to this config for chaining.
         */
        RenderableConfig& shape(std::shared_ptr<helios::rendering::asset::shape::Shape> shape) {
            shape_ = shape;

            return *this;
        }

        /**
         * @brief Sets the shader program.
         *
         * @param shader Shared pointer to the OpenGL shader.
         *
         * @return Reference to this config for chaining.
         */
        RenderableConfig& shader(std::shared_ptr<helios::ext::opengl::rendering::shader::OpenGLShader> shader) {
            shader_ = shader;

            return *this;
        }

        /**
         * @brief Sets the primitive type for rendering.
         *
         * @param primitiveType The OpenGL primitive type.
         *
         * @return Reference to this config for chaining.
         */
        RenderableConfig& primitiveType(helios::rendering::model::config::PrimitiveType primitiveType) {
            primitiveType_ = primitiveType;

            return *this;
        }

        /**
         * @brief Sets the base color for the material.
         *
         * @param color RGBA color vector.
         *
         * @return Reference to this config for chaining.
         */
        RenderableConfig& color(const helios::math::vec4f color) {
            color_ = color;

            return *this;
        }

        /**
         * @brief Builds the renderable and attaches it to a scene node.
         *
         * @param parent Parent SceneNode for the new node.
         */
        void attachTo(helios::scene::SceneNode* parent) {

            build();

            SceneNodeConfig scn{gameObject_};

            scn.parent(parent);
        }

        /**
         * @brief Builds and attaches the RenderableComponent.
         *
         * @return Reference to this config for chaining.
         */
        RenderableConfig& build() {

            assert(shape_ && shader_ && "Unexpected missing shape and/or shader");

            auto mesh = std::make_shared<helios::ext::opengl::rendering::model::OpenGLMesh>(
                *shape_,
                std::move(RenderableConfig::meshConfig(primitiveType_))
            );

            auto materialProperties = std::make_shared<helios::rendering::model::config::MaterialProperties>(color_);
            auto material = std::make_shared<helios::rendering::model::Material>(shader_, materialProperties);

            const auto renderPrototype = std::make_shared<helios::rendering::RenderPrototype>(material, mesh);

            gameObject_->add<helios::engine::modules::rendering::renderable::components::RenderableComponent>(
                std::make_shared<helios::rendering::Renderable>(renderPrototype)
            );

            return *this;
        }

    };

}
