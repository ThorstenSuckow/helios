module;

export module helios.rendering.components.RenderPrototypeComponent;

import helios.rendering.mesh;
import helios.rendering.shader;
import helios.rendering.material;

using namespace helios::rendering::mesh::types;
using namespace helios::rendering::shader::types;
using namespace helios::rendering::material::types;

export namespace helios::rendering::components {

    template<typename THandle>
    class RenderPrototypeComponent {

        MeshHandle meshHandle_;

        ShaderHandle shaderHandle_;

        MaterialHandle materialHandle_;


    public:

        explicit RenderPrototypeComponent(
            const MeshHandle meshHandle,
            const ShaderHandle shaderHandle,
            const MaterialHandle materialHandle
        ): meshHandle_(meshHandle),
        shaderHandle_(shaderHandle),
        materialHandle_(materialHandle)
        {};

        [[nodiscard]] MeshHandle meshHandle() const noexcept {
            return meshHandle_;
        }

        [[nodiscard]] ShaderHandle shaderHandle() const noexcept {
            return shaderHandle_;
        }

        [[nodiscard]] MaterialHandle materialHandle() const noexcept {
            return materialHandle_;
        }


    };
}