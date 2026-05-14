/**
 * @file FramebufferSize.ixx
 * @brief Value type describing a framebuffer size in pixels.
 */
module;

export module helios.rendering.framebuffer.types.FramebufferSize;

import helios.math;

using namespace helios::math;
export namespace helios::rendering::framebuffer::types {

    /**
     * @brief Width/height pair used for framebuffer resize operations.
     */
    using FramebufferSize = helios::math::vec2f;

} // namespace helios::rendering::platform::types
