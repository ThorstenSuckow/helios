/**
 * @file TextRenderCommand.ixx
 * @brief Low-level render command for text rendering.
 */
module;

#include <memory>
#include <string_view>
#include <cassert>
#include <string>

export module helios.rendering.text.TextRenderCommand;


import helios.rendering.shader.UniformValueMap;
import helios.rendering.text.TextRenderPrototype;
import helios.rendering.text.DrawProperties;


export namespace helios::rendering::text {

    /**
     * @brief Immutable command object for rendering a single piece of text.
     *
     * `TextRenderCommand` is a low-level, move-only object that encapsulates all
     * data needed to render text in a single frame. It is created from a
     * `TextRenderable` and consumed by the `TextRenderer`.
     *
     * ## Design
     *
     * - **Immutable:** Once created, the command cannot be modified.
     * - **Move-only:** Copy operations are deleted to prevent accidental duplication.
     * - **Self-contained:** Contains all uniform values needed for rendering.
     *
     * ## Data Flow
     *
     * ```
     * TextRenderable → TextRenderCommand → RenderQueue → TextRenderer
     * ```
     *
     * @see TextRenderable
     * @see TextRenderer
     * @see DrawProperties
     */
    class TextRenderCommand {

        /**
         * @brief Non-owning pointer to the render prototype.
         *
         * The prototype must remain valid for the lifetime of this command.
         */
        const helios::rendering::text::TextRenderPrototype* textRenderPrototype_;

        /**
         * @brief Uniform values specific to this text instance.
         */
        helios::rendering::shader::UniformValueMap textUniformValues_;

        /**
         * @brief The text string to render.
         */
        std::string text_;

        /**
         * @brief Positioning and styling data.
         */
        helios::rendering::text::DrawProperties drawProperties_;

        public:

        TextRenderCommand(const TextRenderCommand&)=delete;
        TextRenderCommand& operator=(const TextRenderCommand&)=delete;

        TextRenderCommand(TextRenderCommand&&) noexcept = default;

        TextRenderCommand& operator=(TextRenderCommand&&) noexcept = default;

        ~TextRenderCommand() = default;

        /**
         * @brief Constructs a text render command.
         *
         * @param text The text string to render.
         * @param textRenderPrototype Non-owning pointer to the prototype (must outlive this command).
         * @param drawProperties Position, scale, and font selection.
         * @param textUniformValues Uniform values for the text shader.
         *
         * @pre `textRenderPrototype` must not be null.
         */
        TextRenderCommand(
            std::string text,
            const helios::rendering::text::TextRenderPrototype* textRenderPrototype,
            const helios::rendering::text::DrawProperties drawProperties,
            const helios::rendering::shader::UniformValueMap& textUniformValues
            ) noexcept :
                text_(std::move(text)),
                textRenderPrototype_(textRenderPrototype),
                drawProperties_(drawProperties),
                textUniformValues_(textUniformValues) {

            assert(textRenderPrototype_ && "TextRenderPrototype must not be null");

        }

        /**
         * @brief Returns the text string.
         *
         * @return View of the text to render.
         */
        [[nodiscard]] std::string_view text() const noexcept {
            return text_;
        }

        /**
         * @brief Returns the render prototype.
         *
         * @return Non-owning pointer to the prototype.
         */
        [[nodiscard]] const helios::rendering::text::TextRenderPrototype* textRenderPrototype() const noexcept {
            return textRenderPrototype_;
        }

        /**
         * @brief Returns the draw properties.
         *
         * @return Reference to positioning and styling data.
         */
        [[nodiscard]] const helios::rendering::text::DrawProperties& drawProperties() const noexcept {
            return drawProperties_;
        }

        /**
         * @brief Returns the text-specific uniform values.
         *
         * @return Reference to the uniform value map.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& textUniformValues() const noexcept {
            return textUniformValues_;
        }
    };


}

