/**
 * @file TypeSetter.ixx
 * @brief Text layout engine for computing glyph positions and bounding boxes.
 */
module;

#include <memory>
#include <span>
#include <string>
#include <limits>
#include <algorithm>
#include <utility>
#include <vector>

export module helios.rendering.text.TypeSetter;


import helios.rendering.text.Glyph;

import helios.rendering.text.FontResourceProvider;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

import helios.util.Colors;
import helios.math;




export namespace helios::rendering::text {

    /**
     * @brief Result of text layout computation.
     *
     * Contains the axis-aligned bounding box and vertex data for a laid-out text string.
     */
    struct TextLayout {

        /**
         * @brief Axis-aligned bounding box enclosing all glyphs.
         */
        math::aabbf aabb;

        /**
         * @brief Vertex data for all glyphs (position and texture coordinates).
         *
         * Each glyph consists of 6 vertices (2 triangles). Each vertex is a `vec4f`
         * containing `(x, y, texU, texV)`.
         */
        std::vector<helios::math::vec4f> vertices;
    };

    /**
     * @brief Computes text layout by positioning glyphs and generating vertex data.
     *
     * The `TypeSetter` is responsible for transforming a text string into renderable
     * geometry. It queries glyph metrics from a `FontResourceProvider` and generates
     * vertex data suitable for GPU rendering.
     *
     * ## Layout Process
     *
     * 1. Iterate over each character in the text string.
     * 2. Query glyph metrics (size, bearing, advance) from the font provider.
     * 3. Compute vertex positions based on the pen position and glyph metrics.
     * 4. Generate two triangles (6 vertices) per glyph with texture coordinates.
     * 5. Advance the pen position by the glyph's advance width.
     * 6. Compute the overall bounding box.
     *
     * ## Vertex Format
     *
     * Each vertex is a `vec4f` containing:
     * - `x, y`: Screen-space position.
     * - `z, w`: Texture coordinates (u, v).
     *
     * ## Usage
     *
     * ```cpp
     * TypeSetter typeSetter;
     * TextLayout layout = typeSetter.layout("Hello", fontId, fontProvider);
     *
     * // Use layout.vertices for rendering
     * // Use layout.aabb for bounds checking
     * ```
     *
     * @see TextLayout
     * @see TextMesh
     * @see FontResourceProvider
     */
    class TypeSetter {


    public:

        /**
         * @brief Computes the layout for a text string.
         *
         * Generates vertex data and computes the bounding box for the given text
         * using the specified font.
         *
         * @param text The text string to lay out.
         * @param fontId The font to use for glyph metrics.
         * @param fontResourceProvider Provider for glyph data.
         *
         * @return A `TextLayout` containing the AABB and vertex data.
         */
        TextLayout layout(
            std::string_view text,
            helios::engine::core::data::FontId fontId,
            FontResourceProvider& fontResourceProvider) {

            helios::math::aabbf aabb = {};

            if (text.empty()) {
                return TextLayout {aabb};
            }

            std::vector<helios::math::vec4f> vertices = {};
            vertices.reserve(text.size() * 6);

            float penX = 0.0f;
            float penY = 0.0f;

            float minX = std::numeric_limits<float>::max();
            float minY = std::numeric_limits<float>::max();
            float minZ = 0.0f;

            float maxX = std::numeric_limits<float>::lowest();
            float maxY = std::numeric_limits<float>::lowest();
            float maxZ = 1.0f;

            for (char c : text) {
                auto glyph = fontResourceProvider.glyph(c, fontId);

                float xpos = penX + glyph.bearing[0];
                float ypos = penY - (glyph.size[1] - glyph.bearing[1]);
                float w = glyph.size[0];
                float h = glyph.size[1];

                vertices.push_back({xpos, ypos +h, 0.0f, 0.0f});
                vertices.push_back({xpos, ypos, 0.0f, 1.0f});
                vertices.push_back({xpos + w, ypos, 1.0f, 1.0f});

                vertices.push_back({xpos, ypos +h, 0.0f, 0.0f});
                vertices.push_back({xpos + w, ypos, 1.0f, 1.0f});
                vertices.push_back({xpos+w, ypos +h, 1.0f, 0.0f});

                minX = std::min(minX, xpos);
                maxX = std::max(maxX, xpos + w);

                minY = std::min(minY, ypos);
                maxY = std::max(maxY, ypos + h);

                penX += (glyph.advance >> 6);
            }

            aabb = helios::math::aabbf(minX, minY, minZ, maxX, maxY, maxZ);

            return {
                aabb, std::move(vertices)
            };
        }



    };


}