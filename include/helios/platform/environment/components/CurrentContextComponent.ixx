/**
 * @file CurrentContextComponent.ixx
 * @brief Marker component identifying the currently active platform context.
 */
module;

#include <string>

export module helios.platform.environment.components.CurrentContextComponent;


export namespace helios::platform::environment::components {

    /**
     * @brief Marker assigned to the entity owning the current platform context.
     *
     * @tparam THandle Runtime platform handle type.
     */
    template<typename THandle>
    struct CurrentContextComponent {};

} // namespace helios::platform::environment::components
