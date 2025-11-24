/**
 * @file ClearFlags.ixx
 *
 * @brief Enum class defining bitmask flags for clearing render buffers before drawing.
 */

export module helios.rendering.ClearFlags;

export namespace helios::rendering {


    /**
     * @brief Defines bitmask flags for clearing render buffers before drawing.
     *
     * These flags can be combined using bitwise OR operations to specify which buffers
     * (color, depth, stencil) should be cleared before rendering.
     *
     * Example usage:
     * ```cpp
     * // Clear both color and depth buffers
     * int clearFlags = std::to_underlying(ClearFlags::Color) | std::to_underlying(ClearFlags::Depth);
     * viewport.setClearFlags(clearFlags);
     * ```
     *
     * @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glClear.xhtml
     */
    enum class ClearFlags {
        /**
         * @brief No buffers are cleared.
         */
        None = 0,

        /**
         * @brief The color buffer is cleared.
         */
        Color = 1 << 0,

        /**
         * @brief The depth buffer is cleared.
         */
        Depth = 1 << 1,

        /**
         * @brief The stencil buffer is cleared.
         */
        Stencil = 1 << 2,
    };


}
