/**
 * @file MeshRenderableConfig.ixx
 * @brief Configuration for renderable components (mesh, material, shader).
 */
module;

#include <memory>
#include <unordered_map>
#include <cassert>

export module helios.engine.builder.gameObject.builders.configs.MeshRenderableConfig;

import helios.rendering;
import helios.ext.opengl;
import helios.rendering;
import helios.math.types;
import helios.rendering.mesh;
import helios.scene.SceneNode;

import helios.engine.builder.gameObject.builders.configs.SceneNodeConfig;


export namespace helios::engine::builder::gameObject::builders::configs {

    /**
     * @brief Fluent configuration for setting up a Renderable on a GameObject.
     *
     * Allows specification of shape, shader, primitive type, and color.
     * Builds and attaches a RenderableComponent.
     */
    template<typename Entity>
    class MeshRenderableConfig {

        using Handle_type = typename Entity::Handle_type;

        /**
         * @brief Non-owning pointer to the target GameObject.
         */
        Entity gameObject_;

        /**
         * @brief The shape geometry to render.
         */
        std::shared_ptr<helios::rendering::asset::shape::Shape> shape_;

        /**
         * @brief The shader for rendering.
         */
        std::shared_ptr<helios::ext::opengl::rendering::shader::LegacyOpenGLShader> shader_;

        /**
         * @brief The primitive type for mesh rendering.
         */
        helios::rendering::mesh::types::PrimitiveType primitiveType_;

        /**
         * @brief The base color for the material.
         */
        helios::math::vec4f color_;

        static inline std::unordered_map<
            helios::rendering::mesh::types::PrimitiveType,
            std::shared_ptr<helios::rendering::mesh::types::MeshConfig>
        > meshConfigs_;

        static std::shared_ptr<helios::rendering::mesh::types::MeshConfig> meshConfig(helios::rendering::mesh::types::PrimitiveType primitiveType) {

            auto [it, inserted] = meshConfigs_.try_emplace(
                primitiveType,
                std::make_shared<helios::rendering::mesh::types::MeshConfig>(primitiveType)
            );

            return it->second;
        }

    public:

        /**
         * @brief Constructs a MeshRenderableConfig for the given GameObject.
         *
         * @param gameObject Target GameObject to configure.
         */
        explicit MeshRenderableConfig(
            Entity gameObject
        ) : gameObject_(gameObject) {}

        /**
         * @brief Sets the shape geometry.
         *
         * @param shape Shared pointer to the shape.
         *
         * @return Reference to this config for chaining.
         */
        MeshRenderableConfig& shape(std::shared_ptr<helios::rendering::asset::shape::Shape> shape) {
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
        MeshRenderableConfig& shader(std::shared_ptr<helios::ext::opengl::rendering::shader::LegacyOpenGLShader> shader) {
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
        MeshRenderableConfig& primitiveType(helios::rendering::mesh::types::PrimitiveType primitiveType) {
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
        MeshRenderableConfig& color(const helios::math::vec4f color) {
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

            SceneNodeConfig<Entity> scn{gameObject_};

            scn.parent(parent);
        }

        /**
         * @brief Builds and attaches the RenderableComponent.
         *
         * @return Reference to this config for chaining.
         */
        MeshRenderableConfig& build() {

            assert(shape_ && shader_ && "Unexpected missing shape and/or shader");

            auto mesh = std::make_shared<helios::ext::opengl::rendering::model::OpenGLMesh>(
                *shape_,
                std::move(MeshRenderableConfig::meshConfig(primitiveType_))
            );

            auto materialProperties = std::make_shared<helios::rendering::material::MaterialShaderProperties>(color_);
            auto material = std::make_shared<helios::rendering::material::Material>(shader_, materialProperties);

            const auto renderPrototype = std::make_shared<helios::rendering::RenderPrototype>(material, mesh);

            auto& rc = gameObject_.template add<helios::rendering::components::RenderableComponent<Handle_type>>(
                std::make_shared<helios::rendering::mesh::MeshRenderable>(renderPrototype)
            );

            auto& msc = gameObject_.template getOrAdd<helios::rendering::model::components::ModelAabbComponent<Handle_type>>();
            msc.setAabb(rc.renderable().localAABB());

            return *this;
        }

    };

}
