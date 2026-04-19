/**
 * @file FramebufferHandle.ixx
 * @brief Entity-handle alias for framebuffer resources.
 */
module;

export module helios.rendering.framebuffer.types.FramebufferHandle;


import helios.ecs.types;
import helios.rendering.framebuffer.types.FramebufferId;

using namespace helios::ecs::types;
export namespace helios::rendering::framebuffer::types {

    /**
     * @brief Handle type used to reference framebuffer entities.
     */
    using FramebufferHandle = helios::ecs::types::EntityHandle<FramebufferId>;

}
