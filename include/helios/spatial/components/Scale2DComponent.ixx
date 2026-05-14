/**
 * @file Scale2DComponent.ixx
 * @brief Alias for 2D scale values.
 */
module;


export module helios.spatial.components.Scale2DComponent;

import helios.core.components.Vec2Component;

export namespace helios::spatial::components {

    /** @brief Domain tag for 2D scale values. */
    struct Scale2DComponentDomain {};

    /**
     * @brief Stores 2D scale data in a `vec2<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using Scale2DComponent = helios::core::components::Vec2Component<Scale2DComponentDomain, THandle, float>;
}