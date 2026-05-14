/**
 * @file Size2DComponent.ixx
 * @brief Alias for 2D size values.
 */
module;


export module helios.spatial.components.Size2DComponent;

import helios.core.components.Vec2Component;

export namespace helios::spatial::components {

    /** @brief Domain tag for 2D size values. */
    struct Size2DComponentDomain {};

    /**
     * @brief Stores 2D size data in a `vec2<float>` component.
     *
     * @tparam THandle Owning entity handle type.
     */
    template<typename THandle>
    using Size2DComponent = helios::core::components::Vec2Component<Size2DComponentDomain, THandle, float>;
}