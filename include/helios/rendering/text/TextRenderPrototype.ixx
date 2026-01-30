/**
 * @file TextRenderPrototype.ixx
 * @brief Shared, immutable configuration for text rendering.
 */
module;

#include <memory>
#include <stdexcept>
#include <string>

export module helios.rendering.text.TextRenderPrototype;


import helios.rendering.text.config.TextShaderProperties;

import helios.rendering.shader.Shader;
import helios.engine.core.data.FontId;

export namespace helios::rendering::text {

    /**
     * @brief Immutable, shared prototype for text rendering configuration.
     *
     * `TextRenderPrototype` holds the shader and text properties that are shared
     * across multiple `TextRenderable` instances. This flyweight pattern reduces
     * memory usage and allows efficient batching of text with the same configuration.
     *
     * ## Design
     *
     * - **Immutable:** Configuration cannot be changed after construction.
     * - **Shared:** Multiple `TextRenderable` objects can reference the same prototype.
     * - **Reduced Duplication:** Avoids redundant storage of shader and property data.
     *
     * ## Usage
     *
     * ```cpp
     * auto shader = std::make_shared<OpenGLShader>(...);
     * auto textProps = std::make_shared<TextShaderProperties>(textColor);
     *
     * auto prototype = std::make_shared<TextRenderPrototype>(shader, textProps);
     *
     * // Share prototype across multiple renderables
     * TextRenderable title("Title", prototype, drawProps1);
     * TextRenderable score("Score: 0", prototype, drawProps2);
     * ```
     *
     * @see TextRenderable
     * @see TextShaderProperties
     * @see Shader
     */
    class TextRenderPrototype final {

    private:

        /**
         * @brief The shader used for rendering text.
         */
        std::shared_ptr <const helios::rendering::shader::Shader> shader_;

        /**
         * @brief Text-specific shader properties (e.g., color).
         */
        std::shared_ptr <const helios::rendering::text::config::TextShaderProperties> textProperties_;



    public:

        /**
         * @brief Constructs a text render prototype.
         *
         * @param shader The shader to use for text rendering.
         * @param textProperties Text-specific shader properties.
         *
         * @throws std::invalid_argument If `shader` is null.
         */
        explicit TextRenderPrototype(
            std::shared_ptr<const helios::rendering::shader::Shader> shader,
            std::shared_ptr<const helios::rendering::text::config::TextShaderProperties> textProperties
        ) :
        shader_(std::move(shader)),
        textProperties_(std::move(textProperties)) {

            if (!shader_) {
                throw std::invalid_argument("RenderPrototype received shader");
            }

        }

        /**
         * @brief Returns the shader.
         *
         * @return Reference to the shader used for text rendering.
         */
        [[nodiscard]] const helios::rendering::shader::Shader& shader() const noexcept {
            return *shader_;
        }

        /**
         * @brief Returns the text shader properties.
         *
         * @return Reference to the text-specific properties.
         */
        [[nodiscard]] const helios::rendering::text::config::TextShaderProperties& textProperties() const noexcept {
            return *textProperties_;
        }

    };


}

