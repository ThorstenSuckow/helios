module;

export module helios.engine.modules.rendering.components.MaterialOverrideComponent;

import helios.rendering.material;

using namespace helios::rendering::material;
using namespace helios::rendering::material::types;
export namespace helios::engine::modules::rendering::components {

    template<typename THandle>
    class MaterialOverrideComponent {


        MaterialHandle materialHandle_;


    public:

        explicit MaterialOverrideComponent(
            const MaterialHandle materialHandle
        ):  materialHandle_(materialHandle)
        {};

        [[nodiscard]] MaterialHandle materialHandle() const noexcept {
            return materialHandle_;
        }

    };

}