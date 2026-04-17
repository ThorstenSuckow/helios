/**
 * @file FramebufferFwd.ixx
 * @brief Forward declaration partition for `helios::rendering::framebuffer::Framebuffer`.
 *
 * @details This partition exists to break include/import coupling between
 * viewport-related code and the full framebuffer module implementation.
 * It is intentionally lightweight and should only provide type forward
 * declarations.
 *
 * This partition is intentionally **not** part of global aggregate exports
 * (for example `helios` or `helios.rendering`) because:
 * - it is an internal dependency-management artifact,
 * - exporting it globally would leak implementation structure,
 * - and consumers that need full framebuffer functionality should import
 *   `helios.rendering.framebuffer` directly.
 */
module;

export module helios.rendering.viewport.Viewport:FramebufferFwd;

export namespace helios::rendering::framebuffer {

    /**
     * @brief Forward declaration of the framebuffer runtime type.
     */
    class Framebuffer;
}