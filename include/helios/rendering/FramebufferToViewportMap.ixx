/**
 * @file FramebufferToViewportMap.ixx
 * @brief Mapping between framebuffer handles and associated viewport handles.
 */
module;

export module helios.rendering.FramebufferToViewportMap;

import helios.core.container.HandleMultiMap;

import helios.rendering.viewport.types.ViewportHandle;
import helios.rendering.framebuffer.types.FramebufferHandle;



using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::core::container;
export namespace helios::rendering {

    /**
     * @brief One-to-many mapping from framebuffer handle to viewport handles.
     */
    using FramebufferToViewportMap = HandleMultiMap<FramebufferHandle, ViewportHandle>;

}