/**
 * @file MaterialOverrideComponent.ixx
 * @brief Stores an explicit material handle override.
 */
module;

export module helios.rendering.common.components.MaterialOverrideComponent;

import helios.rendering.material;

using namespace helios::rendering::material;
using namespace helios::rendering::material::types;
export namespace helios::rendering::common::components {

    /**
     * @brief Component that stores a replacement material handle.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    class MaterialOverrideComponent {


        MaterialHandle materialHandle_{};


    public:

        /**
         * @brief Constructs the component from a material handle.
         *
         * @param materialHandle Override material handle.
         */
        explicit MaterialOverrideComponent(const MaterialHandle materialHandle)
            : materialHandle_(materialHandle) {}

        /** @brief Returns the stored override material handle. */
        [[nodiscard]] MaterialHandle materialHandle() const noexcept {
            return materialHandle_;
        }

    };

}