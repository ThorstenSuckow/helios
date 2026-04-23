module;

export module helios.spatial.components.ComputedLocalToWorldMatrixComponent;

import helios.core.components.Mat4Component;

using namespace helios::core::components;
export namespace helios::spatial::components {

    /**
     * @brief Domain tag for computed local-to-world matrices.
     */
    struct ComputedLocalToWorldMatrixTag {};

    /**
     * @brief Matrix component storing computed local-to-world transforms.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using ComputedLocalToWorldMatrixComponent = Mat4Component<ComputedLocalToWorldMatrixTag, THandle, float>;

}