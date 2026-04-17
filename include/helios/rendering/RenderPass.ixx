/**
 * @file RenderPass.ixx
 * @brief Render-pass payload grouping target handles and extracted render contexts.
 */
module;

#include <vector>

export module helios.rendering.RenderPass;

import helios.rendering.viewport.types.ViewportHandle;
import helios.rendering.framebuffer.types.FramebufferHandle;
import helios.scene.types.SceneMemberRenderContext;

using helios::rendering::viewport::types::ViewportHandle;
using helios::rendering::framebuffer::types::FramebufferHandle;
using namespace helios::scene::types;

export namespace helios::rendering {

    /**
     * @brief DTO representing one render pass for a framebuffer/viewport pair.
     *
     * @tparam THandle Entity handle type used by scene member render contexts.
     */
    template<typename THandle>
    struct RenderPass {

        /**
         * @brief Target viewport for this pass.
         */
        ViewportHandle viewportHandle;

        /**
         * @brief Target framebuffer for this pass.
         */
        FramebufferHandle framebufferHandle;

        /**
         * @brief Scene members to render in this pass.
         */
        std::vector<SceneMemberRenderContext<THandle>> renderContexts;

    };


}