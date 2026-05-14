/**
 * @file RenderPassContext.ixx
 * @brief Lightweight context object for render pass target selection.
 */
module;


export module helios.rendering.common.types.RenderPassContext;


import helios.rendering.common.types.ClearFlags;

import helios.math;

import helios.rendering.framebuffer.types.FramebufferHandle;
import helios.rendering.viewport.types.ViewportHandle;

import helios.rendering.common.types.ClearFlags;

using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::rendering::common::types;
export namespace helios::rendering::common::types {

    /**
     * @brief Groups the target handles used for a render pass.
     */
    struct RenderPassContext {

        /** @brief Target framebuffer handle. */
        FramebufferHandle framebufferHandle{};

        /** @brief Target viewport handle. */
        ViewportHandle viewportHandle{};

    };


}