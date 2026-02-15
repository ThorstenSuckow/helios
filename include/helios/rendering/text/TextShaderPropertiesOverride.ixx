/**
 * @file TextShaderPropertiesOverride.ixx
 * @brief Optional overrides for text shader properties.
 */
module;

#include <optional>

export module helios.rendering.text.TextShaderPropertiesOverride;


import helios.math.types;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;


export namespace helios::rendering::text {

    /**
     * @brief Optional overrides for text shader properties.
     *
     * `TextShaderPropertiesOverride` allows individual `TextRenderable` instances
     * to override specific properties defined in the shared `TextShaderProperties`.
     * Only the properties that are set (non-nullopt) will override the base values.
     *
     * ## Design
     *
     * This pattern allows sharing a common `TextRenderPrototype` while still
     * enabling per-instance customization:
     *
     * ```cpp
     * // Shared prototype with white text
     * auto prototype = std::make_shared<TextRenderPrototype>(shader, textProps, fontProvider);
     *
     * // Override color for a specific renderable
     * TextShaderPropertiesOverride override;
     * override.baseColor = helios::util::Colors::Red;
     *
     * TextRenderable highlight(std::move(mesh), prototype, override);
     * ```
     *
     * @see TextShaderProperties
     * @see TextRenderable
     */
    struct TextShaderPropertiesOverride {

        /**
         * @brief Optional override for the text color.
         *
         * When set, this color overrides the base color from `TextShaderProperties`.
         */
        std::optional<helios::math::vec4f> baseColor;

        /**
         * @brief Creates a new override with the given base color.
         *
         * @param color The color to set.
         *
         * @return A new TextShaderPropertiesOverride with the color set.
         */
        [[nodiscard]] TextShaderPropertiesOverride withBaseColor(helios::math::vec4f color) noexcept {
            return TextShaderPropertiesOverride(color);
        }

        /**
         * @brief Writes override values to a uniform value map.
         *
         * Only writes values that are set (non-nullopt). This method should be
         * called after the base `TextShaderProperties::writeUniformValues()` to
         * apply the overrides.
         *
         * @param uniformValueMap The uniform value map to write to.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {

            if (baseColor) {
                uniformValueMap.set(helios::rendering::shader::UniformSemantics::TextColor, *baseColor);
            }

        }

    };
}
