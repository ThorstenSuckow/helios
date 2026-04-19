/**
 * @file ViewportHandle.ixx
 * @brief Entity-handle alias for viewport resources.
 */
module;

export module helios.rendering.viewport.types.ViewportHandle;


import helios.ecs.types;
import helios.rendering.viewport.types.ViewportId;

using namespace helios::ecs::types;
export namespace helios::rendering::viewport::types {

    /**
     * @brief Handle type used to reference viewport entities.
     */
    using ViewportHandle = helios::ecs::types::EntityHandle<ViewportId>;

}
