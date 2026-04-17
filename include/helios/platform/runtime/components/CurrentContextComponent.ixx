/**
 * @file CurrentContextComponent.ixx
 * @brief Marker component identifying the currently active platform context.
 */
module;

#include <string>

export module helios.platform.runtime.components.CurrentContextComponent;


export namespace helios::platform::runtime::components {

    /**
     * @brief Marker assigned to the entity owning the current platform context.
     *
     * @tparam THandle Runtime platform handle type.
     */
    template<typename THandle>
    struct CurrentContextComponent {};

} // namespace helios::platform::runtime::components
