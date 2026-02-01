/**
 * @file TextRenderer.ixx
 * @brief Abstract base class for text rendering in the helios engine.
 */
module;

#include <string_view>

export module helios.rendering.text.TextRenderer;

import helios.rendering.text.TextRenderCommand;

import helios.engine.core.data.FontId;

import helios.rendering.shader.UniformValueMap;

export namespace helios::rendering::text {

    /**
     * @brief Abstract interface for text rendering implementations.
     *
     * `TextRenderer` defines the contract for rendering text in the helios engine.
     * Concrete implementations handle the specifics of text rendering for different
     * graphics APIs (e.g., OpenGL, Vulkan).
     *
     * ## Responsibilities
     *
     * - **Font Management:** Load and cache font families for efficient glyph rendering.
     * - **Text Rendering:** Process `TextRenderCommand` objects and render text to screen.
     *
     * ## Usage
     *
     * Implementations must override both pure virtual methods:
     *
     * ```cpp
     * class MyTextRenderer : public TextRenderer {
     * public:
     *     void addFontFamily(FontId fontId, std::string_view path,
     *                        unsigned short begin, unsigned short end) override {
     *         // Load font and cache glyphs
     *     }
     *
     *     void render(const TextRenderCommand& command,
     *                 const UniformValueMap& frameUniformValues) noexcept override {
     *         // Render text based on command properties
     *     }
     * };
     * ```
     *
     * @see OpenGLGlyphTextRenderer
     * @see TextRenderCommand
     */
    class TextRenderer {
    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~TextRenderer() = default;

        /**
         * @brief Renders text based on the given command.
         *
         * Processes a `TextRenderCommand` and renders the specified text using the
         * associated shader and font. The implementation should apply both the
         * command-specific uniform values and the frame-level uniform values.
         *
         * @param command The render command containing text, position, scale, and shader information.
         * @param frameUniformValues Frame-level uniform values (e.g., projection matrix) to apply.
         *
         * @note This method is marked `noexcept` and must not throw exceptions.
         */
        virtual void render(
            const TextRenderCommand& command,
            const helios::rendering::shader::UniformValueMap& frameUniformValues
        ) noexcept = 0;
    };
}