/**
 * @file FramebufferId.ixx
 * @brief Strongly-typed identifier for framebuffer resources.
 */
module;

export module helios.rendering.framebuffer.types.FramebufferId;

import helios.core.types.StrongId;
import helios.rendering.framebuffer.types.FramebufferDomainTag;

export namespace helios::rendering::framebuffer::types {

    /**
     * @brief Strongly-typed identifier for framebuffer resources.
     */
    using FramebufferId = helios::core::types::StrongId<FramebufferDomainTag>;

}
