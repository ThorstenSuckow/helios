/**
 * @file DrawProperties.ixx
 * @brief Properties for positioning and styling text during rendering.
 */
module;

#include <memory>
#include <string>
#include <utility>

export module helios.rendering.text.DrawProperties;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

import helios.util.Colors;
import helios.math;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::text {

    /**
     * @brief Properties defining how text should be drawn on screen.
     *
     * `DrawProperties` encapsulates the positioning and styling information
     * needed to render text at a specific location with a specific font.
     *
     * ## Properties
     *
     * - **fontId:** Identifies which loaded font family to use.
     * - **position:** Screen-space coordinates (x, y) for the text origin.
     * - **scale:** Multiplier for the text size (1.0 = original size).
     *
     * ## Example
     *
     * ```cpp
     * DrawProperties props{
     *     .fontId = FontId{1},
     *     .position = {100.0f, 200.0f},
     *     .scale = 1.5f
     * };
     * ```
     *
     * @see TextRenderCommand
     * @see TextRenderer
     */
    struct DrawProperties {

        /**
         * @brief Identifier of the font family to use for rendering.
         *
         * Must reference a font previously loaded via `TextRenderer::addFontFamily()`.
         */
        helios::engine::core::data::FontId fontId;

        /**
         * @brief Screen-space position (x, y) for the text origin.
         *
         * The origin is typically the baseline-left of the first character.
         */
        helios::math::vec2f position;

        /**
         * @brief Scale factor for the rendered text.
         *
         * A value of 1.0 renders at the font's native size.
         * Values > 1.0 enlarge the text, values < 1.0 shrink it.
         */
        float scale = 1.0f;

    };


}