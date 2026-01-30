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

        /**
         * @brief Registers a font family for text rendering.
         *
         * Loads a font from the specified path and caches glyphs within the given
         * character range. The font can later be referenced by its `FontId` when
         * creating `TextRenderCommand` objects.
         *
         * @param fontId Unique identifier for this font family.
         * @param path File system path to the font file (e.g., TTF, OTF).
         * @param begin Start of the character range to load (inclusive, default: 0).
         * @param end End of the character range to load (exclusive, default: 128).
         *
         * @throws std::runtime_error If the font file cannot be loaded.
         * @throws std::invalid_argument If `begin > end`.
         */
        virtual void addFontFamily(
            const helios::engine::core::data::FontId fontId,
            std::string_view path,
            const unsigned short begin = 0,
            const unsigned short end = 128) = 0;
    };





}