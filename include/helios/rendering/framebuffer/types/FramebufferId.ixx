/**
 * @file FramebufferId.ixx
 * @brief Strongly-typed identifier for framebuffer resources.
 */
module;

export module helios.rendering.framebuffer.types.FramebufferId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::rendering::framebuffer::types {

    /**
     * @brief Tag type for FramebufferId.
     */
    struct FramebufferIdTag{};

    /**
     * @brief Strongly-typed identifier for framebuffer resources.
     */
    using FramebufferId = helios::core::types::StrongId<FramebufferIdTag>;

}
