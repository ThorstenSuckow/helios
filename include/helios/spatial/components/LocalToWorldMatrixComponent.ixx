/**
 * @file LocalToWorldMatrixComponent.ixx
 * @brief Alias for computed local-to-world matrices.
 */
module;

export module helios.spatial.components.LocalToWorldMatrixComponent;

import helios.core.components.Mat4Component;

using namespace helios::core::components;
export namespace helios::spatial::components {

    /**
     * @brief Domain tag for computed local-to-world matrices.
     */
    struct LocalToWorldMatrixTag {};

    /**
     * @brief Matrix component storing computed local-to-world transforms.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using LocalToWorldMatrixComponent = Mat4Component<LocalToWorldMatrixTag, THandle, float>;

}