/**
 * @file ProjectionMatrixComponent.ixx
 * @brief Alias for a computed projection matrix component.
 */
module;

export module helios.scene.components.ProjectionMatrixComponent;

import helios.core.components.Mat4Component;

using namespace helios::core::components;
export namespace helios::scene::components {

    /** @brief Domain tag for computed projection matrix values. */
    struct ProjectionMatrixTag {};

    /**
     * @brief Stores a computed projection matrix for a scene-related entity.
     *
     * @tparam TOwnerHandle Owning entity handle type.
     */
    template<typename TOwnerHandle>
    using ProjectionMatrixComponent = Mat4Component<ProjectionMatrixTag, TOwnerHandle, float>;


}