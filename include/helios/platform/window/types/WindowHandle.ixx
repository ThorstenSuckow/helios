/**
 * @file WindowHandle.ixx
 * @brief Handle alias for window entities.
 */
module;

export module helios.platform.window.types.WindowHandle;

import helios.ecs.types.EntityHandle;
import helios.platform.window.types.WindowDomainTag;

export namespace helios::platform::window::types {

    /**
     * @brief ECS handle type referencing window entities.
     */
    using WindowHandle = helios::ecs::types::EntityHandle<WindowDomainTag>;

} // namespace helios::platform::window::types