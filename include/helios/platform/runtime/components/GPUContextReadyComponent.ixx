/**
 * @file GPUContextReadyComponent.ixx
 * @brief Marker component signaling GPU context readiness.
 */
module;

#include <string>

export module helios.platform.runtime.components.GPUContextReadyComponent;


export namespace helios::platform::runtime::components {

    /**
     * @brief Marker set once GPU/context-dependent resources can be used.
     *
     * @tparam THandle Runtime platform handle type.
     */
    template<typename THandle>
    struct GPUContextReadyComponent {};

} // namespace helios::platform::runtime::components
