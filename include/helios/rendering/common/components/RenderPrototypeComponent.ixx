/**
 * @file RenderPrototypeComponent.ixx
 * @brief Groups mesh, shader, and material handles into one render prototype.
 */
module;

export module helios.rendering.common.components.RenderPrototypeComponent;

import helios.rendering.mesh;
import helios.rendering.shader;
import helios.rendering.material;


using namespace helios::rendering::mesh::types;
using namespace helios::rendering::shader::types;
using namespace helios::rendering::material::types;
using namespace helios::rendering::mesh;
using namespace helios::rendering::shader;
using namespace helios::rendering::material;
export namespace helios::rendering::common::components {

    /**
     * @brief Component that stores the resource handles needed for rendering.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    class RenderPrototypeComponent {

        ShaderHandle shaderHandle_;

        MaterialHandle materialHandle_;

        MeshHandle meshHandle_;

    public:

        /**
         * @brief Constructs the component from explicit resource handles.
         *
         * @param shaderHandle Shader handle.
         * @param materialHandle Material handle.
         * @param meshHandle Mesh handle.
         */
        explicit RenderPrototypeComponent(
            const ShaderHandle shaderHandle,
            const MaterialHandle materialHandle,
            const MeshHandle meshHandle
        )
            : shaderHandle_(shaderHandle),
              materialHandle_(materialHandle),
              meshHandle_(meshHandle) {}

        /**
         * @brief Constructs the component from resource entities.
         *
         * @param shader Shader entity.
         * @param material Material entity.
         * @param mesh Mesh entity.
         */
        explicit RenderPrototypeComponent(
            const ShaderEntity shader,
            const MaterialEntity material,
            const MeshEntity mesh
        )
            : shaderHandle_(shader.handle()),
              materialHandle_(material.handle()),
              meshHandle_(mesh.handle()) {}

        /** @brief Returns the mesh handle. */
        [[nodiscard]] MeshHandle meshHandle() const noexcept {
            return meshHandle_;
        }

        /** @brief Returns the shader handle. */
        [[nodiscard]] ShaderHandle shaderHandle() const noexcept {
            return shaderHandle_;
        }

        /** @brief Returns the material handle. */
        [[nodiscard]] MaterialHandle materialHandle() const noexcept {
            return materialHandle_;
        }


    };
}