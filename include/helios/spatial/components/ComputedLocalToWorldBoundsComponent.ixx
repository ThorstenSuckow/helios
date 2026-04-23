module;

export module helios.spatial.components.ComputedLocalToWorldBoundsComponent;

import helios.core.components.AABBComponent;

using namespace helios::core::components;
export namespace helios::spatial::components {

    /**
     * @brief Domain tag for computed world-space bounds.
     */
    struct ComputedLocalToWorldBoundsComponentTag {};

    /**
     * @brief AABB component storing bounds transformed from local to world space.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using ComputedLocalToWorldBoundsComponent = AABBComponent<ComputedLocalToWorldBoundsComponentTag, THandle, float>;

}