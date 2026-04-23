module;

export module helios.spatial.components.LocalPositionStateComponent;

import helios.core.components.Vec3Component;

using namespace helios::core::components;
export namespace helios::spatial::components {

    /**
     * @brief Domain tag for local position state.
     */
    struct LocalPositionStateTag {};

    /**
     * @brief Vector component storing local-space position state.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using LocalPositionStateComponent = Vec3Component<LocalPositionStateTag, THandle, float>; 

}