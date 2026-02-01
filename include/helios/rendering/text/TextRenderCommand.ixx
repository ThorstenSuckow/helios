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
import helios.rendering.text.TextMesh;

import helios.engine.core.data.FontId;

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
     * @see TextMesh
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
        helios::rendering::shader::UniformValueMap objectUniformValues_;

        /**
         * @brief Material/text-specific uniform values (e.g., color).
         */
        helios::rendering::shader::UniformValueMap materialUniformValues_;

        /**
         * @brief Non-owning pointer to the text mesh containing vertex data.
         *
         * The text mesh must remain valid for the lifetime of this command.
         */
        const helios::rendering::text::TextMesh* textMesh_;


        public:

        /**
         * @brief Deleted copy constructor.
         *
         * TextRenderCommand is move-only to prevent accidental duplication.
         */
        TextRenderCommand(const TextRenderCommand&)=delete;

        /**
         * @brief Deleted copy assignment operator.
         *
         * TextRenderCommand is move-only to prevent accidental duplication.
         */
        TextRenderCommand& operator=(const TextRenderCommand&)=delete;

        /**
         * @brief Move constructor.
         */
        TextRenderCommand(TextRenderCommand&&) noexcept = default;

        /**
         * @brief Move assignment operator.
         */
        TextRenderCommand& operator=(TextRenderCommand&&) noexcept = default;

        /**
         * @brief Default destructor.
         */
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
            const helios::rendering::text::TextMesh* textMesh,
            const helios::rendering::text::TextRenderPrototype* textRenderPrototype,
            helios::rendering::shader::UniformValueMap objectUniformValues,
            helios::rendering::shader::UniformValueMap materialUniformValues
            ) noexcept :
                textMesh_(textMesh),
                textRenderPrototype_(textRenderPrototype),
                objectUniformValues_(std::move(objectUniformValues)),
                materialUniformValues_(std::move(materialUniformValues)) {

            assert(textRenderPrototype_ && "TextRenderPrototype must not be null");

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
         * @brief Returns the text mesh containing vertex data.
         *
         * The mesh provides access to the pre-computed glyph vertices for rendering.
         *
         * @return Non-owning pointer to the text mesh.
         */
        [[nodiscard]] const helios::rendering::text::TextMesh* textMesh() const noexcept {
            return textMesh_;
        }

        /**
         * @brief Returns the text-specific uniform values.
         *
         * @return Reference to the uniform value map.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& objectUniformValues() const noexcept {
            return objectUniformValues_;
        }

        /**
         * @brief Returns the material-specific uniform values.
         *
         * Contains values such as text color and other material properties.
         *
         * @return Reference to the material uniform value map.
         */
        [[nodiscard]] const helios::rendering::shader::UniformValueMap& materialUniformValues() const noexcept {
            return materialUniformValues_;
        }
    };


}

