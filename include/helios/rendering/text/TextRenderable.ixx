/**
 * @file TextRenderable.ixx
 * @brief High-level text component for scene integration.
 */
module;

#include <memory>
#include <optional>
#include <stdexcept>
#include <utility>

export module helios.rendering.text.TextRenderable;

import helios.rendering.text.TextRenderPrototype;
import helios.rendering.text.config.TextShaderPropertiesOverride;
import helios.rendering.text.DrawProperties;

import helios.rendering.shader.UniformValueMap;


export namespace helios::rendering::text {

    /**
     * @brief High-level text component that can be attached to game objects.
     *
     * `TextRenderable` represents a piece of text that can be rendered in the scene.
     * It combines a `TextRenderPrototype` (shader and font configuration) with
     * instance-specific data like the text string and positioning.
     *
     * ## Responsibilities
     *
     * - Hold the text string to be rendered.
     * - Reference a shared `TextRenderPrototype` for shader and font settings.
     * - Store instance-specific `DrawProperties` (position, scale, font).
     * - Allow property overrides via `TextShaderPropertiesOverride`.
     *
     * ## Usage
     *
     * ```cpp
     * auto prototype = std::make_shared<TextRenderPrototype>(shader, textProps);
     * DrawProperties drawProps{FontId{1}, {100.0f, 200.0f}, 1.0f};
     *
     * TextRenderable text("Score: 0", prototype, drawProps);
     * text.setText("Score: 100");  // Update dynamically
     * ```
     *
     * @see TextRenderPrototype
     * @see TextRenderCommand
     * @see DrawProperties
     */
    class TextRenderable final {

    protected:

        /**
         * @brief Shared prototype containing shader and text properties.
         */
        std::shared_ptr<const helios::rendering::text::TextRenderPrototype> textRenderPrototype_ = nullptr;

        /**
         * @brief The text string to render.
         */
        std::string text_;

        /**
         * @brief Optional overrides for text shader properties.
         */
        std::optional<helios::rendering::text::config::TextShaderPropertiesOverride> textPropertiesOverride_;

        /**
         * @brief Positioning and styling data (font, position, scale).
         */
        helios::rendering::text::DrawProperties drawProperties_;


    public:

        /**
         * @brief Constructs a TextRenderable with the given text and configuration.
         *
         * @param text The text string to render.
         * @param renderPrototype Shared prototype with shader and font configuration.
         * @param drawProperties Position, scale, and font selection.
         * @param textPropertiesOverride Optional overrides for shader properties.
         *
         * @throws std::invalid_argument If `renderPrototype` is null.
         */
        explicit TextRenderable(
            std::string text,
            std::shared_ptr<const helios::rendering::text::TextRenderPrototype> renderPrototype,
            const helios::rendering::text::DrawProperties drawProperties,
            const std::optional<helios::rendering::text::config::TextShaderPropertiesOverride>& textPropertiesOverride = std::nullopt

        ) :
            text_(std::move(text)),
            textRenderPrototype_(std::move(renderPrototype)),
            drawProperties_(drawProperties),
            textPropertiesOverride_(textPropertiesOverride)
        {

            if (!textRenderPrototype_) {
                throw std::invalid_argument("Unexpected nullptr for TextRenderPrototype");
            }

        }

        /**
         * @brief Updates the text string.
         *
         * @param text The new text to render.
         */
        void setText(std::string text) noexcept {
            text_ = std::move(text);
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
         * @brief Returns the text render prototype.
         *
         * @return Shared pointer to the prototype.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::text::TextRenderPrototype> textRenderPrototype() const noexcept {
            return textRenderPrototype_;
        }

        /**
         * @brief Returns the draw properties.
         *
         * @return Reference to the positioning and styling data.
         */
        [[nodiscard]] const helios::rendering::text::DrawProperties& drawProperties() const noexcept {
            return drawProperties_;
        }

        /**
         * @brief Writes uniform values to the given map.
         *
         * Applies the prototype's text properties first, then any instance-specific
         * overrides. Used by the render pipeline to configure the text shader.
         *
         * @param uniformValueMap The map to write uniform values to.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept {
            textRenderPrototype_->textProperties().writeUniformValues(uniformValueMap);

            if (textPropertiesOverride_) {
                textPropertiesOverride_->writeUniformValues(uniformValueMap);
            }
        }



    };

}

