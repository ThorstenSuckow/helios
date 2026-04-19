/**
 * @file GPUContextReadyComponent.ixx
 * @brief Marker component signaling GPU context readiness.
 */
module;

#include <string>

export module helios.platform.environment.components.GPUContextReadyComponent;


export namespace helios::platform::environment::components {

    /**
     * @brief Marker set once GPU/context-dependent resources can be used.
     *
     * @tparam THandle Runtime platform handle type.
     */
    template<typename THandle>
    struct GPUContextReadyComponent {};

} // namespace helios::platform::environment::components
