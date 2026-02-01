/**
 * @file TextShaderProperties.ixx
 * @brief Defines shader properties for text rendering.
 */
module;

#include <memory>
#include <string>
#include <utility>

export module helios.rendering.text.config.TextShaderProperties;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

import helios.util.Colors;
import helios.math;

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

export namespace helios::rendering::text::config {

    /**
     * @brief Immutable shader properties for text rendering.
     *
     * `TextShaderProperties` defines the base visual properties for rendering text,
     * such as the text color. These properties are typically shared across multiple
     * `TextRenderable` instances via a `TextRenderPrototype`.
     *
     * ## Usage
     *
     * ```cpp
     * // Create text properties with a custom color
     * auto textProps = std::make_shared<TextShaderProperties>(
     *     helios::util::Colors::Red
     * );
     *
     * // Use with a render prototype
     * auto prototype = std::make_shared<TextRenderPrototype>(shader, textProps, fontProvider);
     * ```
     *
     * @see TextRenderPrototype
     * @see TextShaderPropertiesOverride
     * @see UniformSemantics::TextColor
     */
    class TextShaderProperties final {

    private:

        /**
         * @brief Base text color (RGBA).
         */
        helios::math::vec4f baseColor_;

    public:

        /**
         * @brief Constructs TextShaderProperties with the specified color.
         *
         * @param baseColor The text color. Defaults to white.
         */
        explicit TextShaderProperties(
            const helios::math::vec4f baseColor = helios::util::Colors::White

        ) : baseColor_(baseColor) {}

        /**
         * @brief Writes the text shader properties to a uniform value map.
         *
         * Sets the `UniformSemantics::TextColor` uniform to the base color.
         *
         * @param uniformValueMap The uniform value map to write to.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
            uniformValueMap.set(helios::rendering::shader::UniformSemantics::TextColor, baseColor_);
        }
    };


}