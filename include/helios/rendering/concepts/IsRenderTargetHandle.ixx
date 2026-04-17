/**
 * @file IsRenderTargetHandle.ixx
 * @brief Concept constraining render target handle types.
 */
module;

#include <concepts>

export module helios.rendering.concepts.IsRenderTargetHandle;

import helios.rendering.viewport.types.ViewportHandle;
import helios.rendering.framebuffer.types.FramebufferHandle;

using namespace helios::rendering::viewport::types;
using namespace helios::rendering::framebuffer::types;
export namespace helios::rendering::concepts {

    /**
     * @brief Constrains types to known render target handles.
     *
     * @tparam T Type to validate.
     */
    template<typename T>
    concept IsRenderTargetHandle = std::is_same_v<T, ViewportHandle> ||
                                     std::is_same_v<T, FramebufferHandle>;

}