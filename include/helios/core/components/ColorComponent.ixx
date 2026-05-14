/**
 * @file ColorComponent.ixx
 * @brief RGBA-style color component alias based on Vec4Component.
 */
module;

export module helios.core.components.ColorComponent;

import helios.core.components.Vec4Component;

export namespace helios::core::components {

    /** @brief Domain tag used for color-valued components. */
    struct ColorDomainTag{};

    /**
     * @brief Convenience alias for 4-channel floating-point color values.
     *
     * @tparam THandle Owning entity handle type.
     *
     * This alias maps color storage to `Vec4Component` with `float` channels,
     * typically interpreted as RGBA.
     */
    template<typename THandle>
    using ColorComponent = helios::core::components::Vec4Component<ColorDomainTag, THandle, float>;

}