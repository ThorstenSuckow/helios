/**
 * @file IsRenderBackendLike.ixx
 * @brief Concept constraining render backend interfaces.
 */
module;

#include <concepts>

export module helios.rendering.common.concepts.IsRenderBackendLike;

import helios.math.types;

import helios.rendering.common.types.RenderPassContext;
import helios.rendering.framebuffer.types.FramebufferHandle;
import helios.rendering.viewport.types.ViewportHandle;
import helios.util.Colors;

import helios.scene.types.SceneMemberRenderContext;


using namespace helios::math;
using namespace helios::rendering::framebuffer::types;
using namespace helios::rendering::viewport::types;
using namespace helios::rendering::common::types;
using namespace helios::scene::types;

export namespace helios::rendering::common::concepts {

    /**
     * @brief Constrains types that provide the rendering backend API surface.
     *
     * @tparam T Backend candidate type.
     * @tparam THandle Scene member handle type used by `doRender`.
     */
    template<typename T, typename THandle>
    concept IsRenderBackendLike = requires(
            T& t,
            const T& ct,
            const FramebufferHandle framebufferHandle,
            const ViewportHandle viewportHandle,
            const vec4f clearColor,
            const RenderPassContext& renderPassContext,
            const SceneMemberRenderContext<THandle>& renderContext
           )
    {
        {t.beginRenderPass(renderPassContext)}->std::same_as<void>;
        {t.doRender(renderContext)}->std::same_as<void>;
        {t.endRenderPass(renderPassContext)}->std::same_as<void>;

    };




}