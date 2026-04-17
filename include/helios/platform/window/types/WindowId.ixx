/**
 * @file WindowId.ixx
 * @brief Strong-id type definition for window domain entities.
 */
module;

export module helios.platform.window.types.WindowId;

import helios.core.types.StrongId;

using namespace helios::core::types;
export namespace helios::platform::window::types {

    /**
     * @brief Tag type used to specialize `StrongId` for windows.
     */
    struct WindowTag{};

    /**
     * @brief Strong id identifying window entities.
     */
    using WindowId = StrongId<WindowTag>;

}