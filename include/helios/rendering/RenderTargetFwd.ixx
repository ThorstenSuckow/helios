/**
 * @file RenderTargetFwd.ixx
 * @brief Provides a forward declaration for the helios::rendering::RenderTarget class.
 */
module;

export module helios.rendering.Viewport:RenderTargetFwd;

export namespace helios::rendering {
    /**
     * @brief Forward declaration of the RenderTarget class.
     *
     * This allows other modules to use pointers or references to RenderTarget
     * without needing the full class definition, reducing compile-time dependencies.
     *
     * @see helios::rendering::RenderTarget
     */
    class RenderTarget;
}