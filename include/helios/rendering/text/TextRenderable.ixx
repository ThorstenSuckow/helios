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

import helios.rendering.Renderable;
import helios.rendering.RenderQueue;

import helios.rendering.text.TextRenderCommand;

import helios.rendering.text.TextRenderPrototype;
import helios.rendering.text.TextShaderPropertiesOverride;
import helios.rendering.text.TextMesh;

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
     * - Store instance-specific `TextMesh` (position, scale, font).
     * - Allow property overrides via `TextShaderPropertiesOverride`.
     *
     * ## Usage
     *
     * ```cpp
     * auto prototype = std::make_shared<TextRenderPrototype>(shader, textProps);
     * TextMesh drawProps{FontId{1}, {100.0f, 200.0f}, 1.0f};
     *
     * TextRenderable text("Score: 0", prototype, drawProps);
     * text.setText("Score: 100");  // Update dynamically
     * ```
     *
     * @see TextRenderPrototype
     * @see TextRenderCommand
     * @see TextMesh
     */
    class TextRenderable final : public helios::rendering::Renderable  {

    protected:

        /**
         * @brief Shared prototype containing shader and text properties.
         */
        std::shared_ptr<const helios::rendering::text::TextRenderPrototype> textRenderPrototype_ = nullptr;


        /**
         * @brief Optional overrides for text shader properties.
         */
        std::optional<helios::rendering::text::TextShaderPropertiesOverride> textPropertiesOverride_;

        /**
         * @brief Positioning and styling data (font, position, scale).
         */
        std::unique_ptr<helios::rendering::text::TextMesh> textMesh_;


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
            std::unique_ptr<helios::rendering::text::TextMesh> textMesh,
            std::shared_ptr<const helios::rendering::text::TextRenderPrototype> renderPrototype,
            const std::optional<helios::rendering::text::TextShaderPropertiesOverride>& textPropertiesOverride = std::nullopt
        ) :
            textRenderPrototype_(std::move(renderPrototype)),
            textMesh_(std::move(textMesh)),
            textPropertiesOverride_(textPropertiesOverride)
        {

            if (!textRenderPrototype_) {
                throw std::invalid_argument("Unexpected nullptr for TextRenderPrototype");
            }

        }

        /**
         * @brief Returns the local-space axis-aligned bounding box.
         *
         * The AABB is computed based on the text layout and font metrics.
         *
         * @return Reference to the local-space AABB.
         */
        [[nodiscard]] const helios::math::aabbf& localAABB() const noexcept override {
            return textMesh_->localAABB(textRenderPrototype_->fontResourceProvider());
        }

        /**
         * @brief Updates the text string.
         *
         * @param text The new text to render.
         */
        void setText(std::string text) noexcept {
            textMesh_->setText(std::move(text));
        }

        /**
         * @brief Returns the current text string.
         *
         * @return View of the text string.
         */
        [[nodiscard]] std::string_view text() const noexcept {
            return textMesh_->text();
        }

        /**
         * @brief Returns the text render prototype.
         *
         * **PERF-NOTE:** This method returns a copy of the `shared_ptr`, triggering atomic
         * reference counting. For read-only access in rendering loops, consider using
         * `textRenderPrototype_.get()` directly or adding a `const TextRenderPrototype&`
         * overload.
         *
         * @return Shared pointer to the prototype.
         */
        [[nodiscard]] std::shared_ptr<const helios::rendering::text::TextRenderPrototype> shareTextRenderPrototype() const noexcept {
            return textRenderPrototype_;
        }

        [[nodiscard]] const helios::rendering::text::TextRenderPrototype* textRenderPrototype() const noexcept {
            return textRenderPrototype_.get();
        }

        /**
         * @brief Returns the text mesh.
         *
         * The text mesh contains the text content, font, and cached layout data.
         *
         * @return Reference to the text mesh.
         */
        [[nodiscard]] const helios::rendering::text::TextMesh& textMesh() const noexcept {
            return *textMesh_;
        }

        /**
         * @brief Writes uniform values to the given map.
         *
         * Applies the prototype's text properties first, then any instance-specific
         * overrides. Used by the render pipeline to configure the text shader.
         *
         * @param uniformValueMap The map to write uniform values to.
         */
        void writeUniformValues(helios::rendering::shader::UniformValueMap& uniformValueMap) const noexcept override {
            textRenderPrototype_->textProperties().writeUniformValues(uniformValueMap);

            if (textPropertiesOverride_) {
                textPropertiesOverride_->writeUniformValues(uniformValueMap);
            }
        }

        /**
         * @brief Emits a text render command to the render queue.
         *
         * Creates a `TextRenderCommand` with the current text, prototype, and uniform
         * values, then adds it to the render queue for processing by the `TextRenderer`.
         *
         * @param renderQueue The render queue to emit the command to.
         * @param objectUniformValues Object-specific uniform values (e.g., model matrix).
         * @param materialUniformValues Material uniform values (will be merged with text properties).
         */
        void emit(
            helios::rendering::RenderQueue& renderQueue,
            helios::rendering::shader::UniformValueMap objectUniformValues,
            helios::rendering::shader::UniformValueMap materialUniformValues) const override {


            writeUniformValues(materialUniformValues);

            renderQueue.add(helios::rendering::text::TextRenderCommand(
                textMesh_.get(),
                textRenderPrototype_.get(),
                std::move(objectUniformValues),
                std::move(materialUniformValues)
            ));
        };


    };

}

