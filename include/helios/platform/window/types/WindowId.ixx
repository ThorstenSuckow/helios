/**
 * @file WindowId.ixx
 * @brief Strong-id type definition for window domain entities.
 */
module;

export module helios.platform.window.types.WindowId;

import helios.core.types.StrongId;
import helios.platform.window.types.WindowDomainTag;

using namespace helios::core::types;
export namespace helios::platform::window::types {

    /**
     * @brief Strong id identifying window entities.
     */
    using WindowId = StrongId<WindowDomainTag>;

}