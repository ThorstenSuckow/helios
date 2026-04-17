/**
 * @file ViewportSnapshot.ixx
 * @brief Immutable snapshot of viewport state for frame-consistent rendering.
 */
module;



export module helios.rendering.viewport.ViewportSnapshot;

import helios.rendering.viewport.types.ViewportHandle;
import helios.math.types;
import helios.core.types;

using namespace helios::rendering::viewport::types;
export namespace helios::rendering::viewport {

    /**
     * @brief Immutable snapshot of a Viewport's state at a specific point in time.
     *
     * ViewportSnapshot captures the essential viewport data needed for rendering,
     * including viewport ID and both normalized and absolute bounds. This allows
     * systems to work with consistent viewport data throughout a frame, even if
     * the source Viewport is modified.
     */
    struct ViewportSnapshot {

        /**
         * @brief The associated viewport's handle.
         */
        ViewportHandle viewportHandle{};

        /**
         * @brief Normalized bounds [x, y, width, height] in range [0, 1].
         */
        helios::math::vec4f bounds;

        /**
         * @brief Absolute bounds [x, y, width, height] in pixels.
         */
        helios::math::vec4f absoluteBounds;
    };


} // namespace helios::rendering::viewport
