/**
 * @file LocalToWorldBoundsComponent.ixx
 * @brief Alias for computed world-space bounds.
 */
module;

export module helios.spatial.components.LocalToWorldBoundsComponent;

import helios.core.components.AABBComponent;

using namespace helios::core::components;
export namespace helios::spatial::components {

    /**
     * @brief Domain tag for computed world-space bounds.
     */
    struct LocalToWorldBoundsComponentTag {};

    /**
     * @brief AABB component storing bounds transformed from local to world space.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using LocalToWorldBoundsComponent = AABBComponent<LocalToWorldBoundsComponentTag, THandle, float>;

}