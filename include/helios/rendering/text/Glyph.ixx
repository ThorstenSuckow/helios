/**
 * @file Glyph.ixx
 * @brief Glyph data structure for text rendering.
 */
module;

export module helios.rendering.text.Glyph;

import helios.math;

export namespace helios::rendering::text {

    /**
     * @brief Represents a single rendered glyph (character) from a font.
     *
     * A `Glyph` contains all the information needed to render a single character:
     * the texture ID, size, bearing (offset from baseline), and advance width.
     *
     * ## Glyph Metrics
     *
     * The following diagram illustrates the glyph metrics:
     *
     * ```
     *     +-----------+
     *     |           |
     *     |   Glyph   |  <-- size.y (height)
     *     |           |
     *     +-----------+ <-- baseline
     *     ^           ^
     *     |           |
     *     bearing.x   |
     *                 advance (to next glyph)
     * ```
     *
     * - **bearing.x:** Horizontal offset from the cursor position to the left edge of the glyph.
     * - **bearing.y:** Vertical offset from the baseline to the top of the glyph.
     * - **advance:** Horizontal distance to move the cursor after rendering this glyph.
     *
     * @see TextRenderer
     * @see OpenGLGlyphTextRenderer
     */
    struct Glyph {

        /**
         * @brief GPU texture ID containing the rendered glyph bitmap.
         */
        unsigned int textureId;

        /**
         * @brief Size of the glyph in pixels (width, height).
         */
        helios::math::vec2ui size;

        /**
         * @brief Offset from baseline to left/top of glyph (bearingX, bearingY).
         *
         * - `bearing.x`: Horizontal offset from cursor to left edge of glyph.
         * - `bearing.y`: Vertical offset from baseline to top of glyph.
         */
        helios::math::vec2i bearing;

        /**
         * @brief Horizontal offset to advance to the next glyph position.
         *
         * The value is stored in 1/64th of a pixel (FreeType format).
         * Use `advance >> 6` to convert to pixels.
         */
        int advance;

    };

}
