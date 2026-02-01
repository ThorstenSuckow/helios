/**
 * @file TextMesh.ixx
 * @brief Properties for positioning and styling text during rendering.
 */
module;

#include <memory>
#include <span>
#include <string>
#include <algorithm>
#include <utility>

export module helios.rendering.text.TextMesh;

import helios.rendering.text.Glyph;
import helios.rendering.text.TypeSetter;

import helios.rendering.text.FontResourceProvider;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

import helios.util.Colors;
import helios.math;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::text {

    /**
     * @brief Manages text content and caches layout data for efficient rendering.
     *
     * `TextMesh` encapsulates a text string along with its font and provides lazy
     * computation of vertex data and bounding boxes. The layout is computed on-demand
     * and cached until the text content changes.
     *
     * ## Caching Strategy
     *
     * - Layout computation is deferred until `vertices()` or `localAABB()` is called.
     * - The cache is invalidated when `setText()` is called.
     * - The `TypeSetter` is used internally to compute glyph positions.
     *
     * ## Usage
     *
     * ```cpp
     * TextMesh mesh("Score: 0", FontId{1});
     *
     * // Access vertices for rendering (triggers layout if needed)
     * auto verts = mesh.vertices(fontProvider);
     *
     * // Update text (invalidates cache)
     * mesh.setText("Score: 100");
     * ```
     *
     * @see TypeSetter
     * @see TextRenderable
     * @see FontResourceProvider
     */
    class TextMesh {

        /**
         * @brief Font identifier for glyph lookup.
         */
        helios::engine::core::data::FontId fontId_;

        /**
         * @brief The text string to render.
         */
        std::string text_;

        /**
         * @brief Flag indicating whether the cached layout needs recomputation.
         */
        mutable bool needsUpdate_ = true;

        /**
         * @brief Cached layout data (AABB and vertices).
         */
        mutable TextLayout textLayout_;

        /**
         * @brief TypeSetter instance for computing text layout.
         */
        mutable TypeSetter typeSetter_;

        /**
         * @brief Updates the cached layout if needed.
         *
         * @param fontResourceProvider Provider for glyph data.
         */
        void updateCache(FontResourceProvider& fontResourceProvider) const {

            if (!needsUpdate_) {
                return;
            }

            textLayout_ = typeSetter_.layout(text_, fontId_, fontResourceProvider);

            needsUpdate_ = false;
        }

    public:

        /**
         * @brief Constructs a TextMesh with the given text and font.
         *
         * @param text The text string to render.
         * @param fontId The font identifier for glyph lookup.
         */
        explicit TextMesh(
            std::string text,
            helios::engine::core::data::FontId fontId
        ) noexcept
        :
            text_(std::move(text)),
            fontId_(fontId) {}


        /**
         * @brief Returns the local-space axis-aligned bounding box.
         *
         * Triggers layout computation if the cache is invalid.
         *
         * @param fontResourceProvider Provider for glyph data.
         *
         * @return Reference to the cached AABB.
         */
        [[nodiscard]] const helios::math::aabbf& localAABB(FontResourceProvider& fontResourceProvider) const noexcept {
            updateCache(fontResourceProvider);
            return textLayout_.aabb;
        }

        /**
         * @brief Returns the vertex data for rendering.
         *
         * Each glyph is represented as 6 vertices (2 triangles). Triggers layout
         * computation if the cache is invalid.
         *
         * @param fontResourceProvider Provider for glyph data.
         *
         * @return A span of vertex data (x, y, texU, texV per vertex).
         */
        [[nodiscard]] std::span<helios::math::vec4f> vertices(FontResourceProvider& fontResourceProvider) const noexcept {
            updateCache(fontResourceProvider);
            return textLayout_.vertices;
        }

        /**
         * @brief Updates the text string.
         *
         * Invalidates the cached layout, which will be recomputed on the next
         * call to `vertices()` or `localAABB()`.
         *
         * @param text The new text to render.
         */
        void setText(std::string text) noexcept {
            text_ = std::move(text);
            needsUpdate_ = true;
        }

        /**
         * @brief Returns the current text string.
         *
         * @return View of the text string.
         */
        [[nodiscard]] std::string_view text() const noexcept {
            return text_;
        }

        /**
         * @brief Returns the font identifier.
         *
         * @return The font ID used for glyph lookup.
         */
        [[nodiscard]] helios::engine::core::data::FontId fontId() const noexcept {
            return fontId_;
        }
    };


}