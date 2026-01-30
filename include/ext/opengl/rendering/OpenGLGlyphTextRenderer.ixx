/**
 * @file OpenGLGlyphTextRenderer.ixx
 * @brief OpenGL-based text renderer using FreeType glyph rendering.
 */
module;

#include <cassert>
#include <ft2build.h>
#include <glad/gl.h>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>

#include FT_FREETYPE_H

export module helios.ext.opengl.rendering.OpenGLGlyphTextRenderer;

import helios.rendering.text.TextRenderer;
import helios.rendering.text.Glyph;

import helios.math;
import helios.ext.opengl.rendering.shader.OpenGLShader;

import helios.engine.core.data.FontId;
import helios.rendering.text.TextRenderCommand;

import helios.rendering.shader.UniformValueMap;

export namespace helios::ext::opengl::rendering {

    /**
     * @brief OpenGL implementation of `TextRenderer` using FreeType for glyph rendering.
     *
     * `OpenGLGlyphTextRenderer` provides hardware-accelerated text rendering using OpenGL.
     * It uses the FreeType library to load font files and rasterize glyphs into textures,
     * which are then rendered as textured quads.
     *
     * ## Features
     *
     * - **FreeType Integration:** Loads TrueType (TTF) and OpenType (OTF) fonts.
     * - **Per-Font Caching:** Caches glyph textures per font family for efficient rendering.
     * - **Dynamic Text:** Updates VBO per character for flexible text positioning.
     * - **Shader Integration:** Works with `OpenGLShader` for customizable text appearance.
     *
     * ## Architecture
     *
     * The renderer maintains:
     * - A **VAO/VBO pair** for rendering glyph quads.
     * - A **font cache** mapping `FontId` to pre-rendered glyph textures.
     *
     * ## Usage
     *
     * ```cpp
     * // Create and initialize the renderer
     * OpenGLGlyphTextRenderer renderer;
     * renderer.init();
     *
     * // Load a font family
     * renderer.addFontFamily(FontId{1}, "fonts/arial.ttf", 32, 127);
     *
     * // Create a text render command
     * TextRenderCommand command(
     *     "Hello, World!",
     *     textPrototype,
     *     DrawProperties{FontId{1}, {100.0f, 200.0f}, 1.0f},
     *     uniformValues
     * );
     *
     * // Render the text
     * renderer.render(command, frameUniforms);
     * ```
     *
     * ## OpenGL State Requirements
     *
     * Before calling `render()`, ensure:
     * - Blending is enabled: `glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);`
     * - The text shader is configured to sample from texture unit 0.
     *
     * ## Performance Considerations
     *
     * - Each character results in a separate draw call (6 vertices per glyph).
     * - For large amounts of text, consider batching or using a texture atlas approach.
     *
     * @see [Vri20, pp. 449-458].
     *
     * @see TextRenderer
     * @see Glyph
     * @see TextRenderCommand
     */
    class OpenGLGlyphTextRenderer : public helios::rendering::text::TextRenderer {

        /**
         * @brief Internal cache for pre-rendered glyph data of a single font.
         */
        struct FontCache {
            /**
             * @brief Array of cached glyphs, indexed by character code offset.
             */
            helios::rendering::text::Glyph characters[128];
        };

        /**
         * @brief Map of font IDs to their cached glyph data.
         */
        std::unordered_map<
            helios::engine::core::data::FontId, FontCache
        > fontCache_;

        friend class OpenGLDevice;

        /**
         * @brief Initializes OpenGL resources (VAO and VBO) for text rendering.
         *
         * Creates a vertex array object and vertex buffer for rendering glyph quads.
         * The VBO is configured for dynamic updates (one quad per character).
         *
         * Must be called before any rendering operations.
         */
        void init() {
            glGenVertexArrays(1, &vao_);
            glGenBuffers(1, &vbo_);
            glBindVertexArray(vao_);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_);
            // 2D quad for a texture requires 6 vertices with 4 floats each => 6*4
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6*4, nullptr, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        /**
         * @brief Loads a font and caches glyph textures for the specified character range.
         *
         * Uses FreeType to load the font file and render each glyph in the specified
         * range to an OpenGL texture.
         *
         * @param fontId Unique identifier for this font.
         * @param pathToFont File system path to the font file.
         * @param begin Start of character range (inclusive).
         * @param end End of character range (exclusive).
         *
         * @throws std::runtime_error If FreeType initialization or font loading fails.
         */
        void loadMap(
            helios::engine::core::data::FontId fontId,
            std::string_view pathToFont,
            const unsigned short begin, const unsigned short end) {

            assert(!fontCache_.contains(fontId) && "FontCache already existing");

            fontCache_.emplace(fontId, FontCache{});

            FT_Library ft{};

            FT_Face face{};

            if (FT_Init_FreeType(&ft)) {
                throw std::runtime_error("ERROR::FREETYPE: Could not init FreeType Library");

            }

            if (FT_New_Face(ft, pathToFont.data(), 0, &face)) {
                throw std::runtime_error("ERROR::FREETYPE: Failed to load font");
            }

            FT_Set_Pixel_Sizes(face, 0, 48);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glPixelStorei(GL_UNPACK_ROW_LENGTH, face->glyph->bitmap.pitch);

            for (unsigned short c = begin; c < end; ++c) {

                if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    throw std::runtime_error("ERROR::FREETYTPE: Failed to load Glyph");
                }

                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_R8,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer
                );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                fontCache_[fontId].characters[c - begin] =  {
                    texture,
                    {face->glyph->bitmap.width, face->glyph->bitmap.rows},
                    {face->glyph->bitmap_left, face->glyph->bitmap_top},
                    face->glyph->advance.x
                };

            }

            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        /**
         * @brief Start of the loaded character range.
         */
        unsigned short begin_{};

        /**
         * @brief End of the loaded character range.
         */
        unsigned short end_{};

        /**
         * @brief OpenGL Vertex Array Object for glyph rendering.
         */
        unsigned int vao_{};

        /**
         * @brief OpenGL Vertex Buffer Object for glyph quad vertices.
         */
        unsigned int vbo_{};

        /**
         * @brief Renders a string of text at the specified position.
         *
         * Iterates through each character in the text, looks up the corresponding
         * glyph from the cache, and renders it as a textured quad.
         *
         * @param text The text string to render.
         * @param position Screen-space position (x, y) for the text origin.
         * @param scale Scaling factor for the text size.
         * @param fontId The font to use for rendering.
         */
        void renderText(
            const std::string_view text,
            helios::math::vec2f position,
            const float scale,
            const helios::engine::core::data::FontId fontId
        )  noexcept {

            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(vao_);
            for (const char c : text) {
                assert(begin_ < 128 && "Glyph range invalid, possible underflow");
                helios::rendering::text::Glyph ch = fontCache_[fontId].characters[c - begin_];
                const float xpos = position[0] + ch.bearing[0] * scale;
                const float ypos = position[1] - ( ch.size[1] - ch.bearing[1]) * scale;

                const float w = ch.size[0] * scale;
                const float h = ch.size[1] * scale;

                // update the VBO for _each_ character (multiple drawcalls!)
                const float vertices[6][4] = {
                    {xpos, ypos +h, 0.0f, 0.0f},
                    {xpos, ypos, 0.0f, 1.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},

                    {xpos, ypos +h, 0.0f, 0.0f},
                    {xpos + w, ypos, 1.0f, 1.0f},
                    {xpos+w, ypos +h, 1.0f, 0.0f}
                };

                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.textureId);
                // update content of vbo memory
                glBindBuffer(GL_ARRAY_BUFFER, vbo_);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                //render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // advance cursors for next glyph (1/64 pixels)
                position[0] += (ch.advance >> 6) * scale;

            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
        }


    public:

        /**
         * @brief Returns the texture unit used for text rendering.
         *
         * @return The texture unit index (always 0).
         */
        static inline int textTextureUnit() {
            return 0;
        }

        /**
         * @brief Registers a font family for text rendering.
         *
         * Loads the font from the specified path and pre-renders glyphs for all
         * characters in the given range. The glyphs are cached as OpenGL textures.
         *
         * @param fontId Unique identifier for this font family.
         * @param path File system path to the font file (TTF, OTF).
         * @param begin Start of character range to load (inclusive, default: 0).
         * @param end End of character range to load (exclusive, default: 128).
         *
         * @throws std::invalid_argument If `begin > end`.
         * @throws std::runtime_error If the font cannot be loaded.
         */
        void addFontFamily(
            const helios::engine::core::data::FontId fontId, std::string_view path,
            const unsigned short begin = 0, const unsigned short end = 128) override {

            if (begin > end) {
                throw std::invalid_argument("Glyph range is invalid");
            }

            loadMap(fontId, path, begin, end);
        }

        /**
         * @brief Renders text using the specified render command.
         *
         * Processes the `TextRenderCommand`, activates the associated shader,
         * applies uniform values, and renders the text string.
         *
         * @param command The render command containing text and rendering properties.
         * @param frameUniformValues Frame-level uniforms (e.g., projection matrix).
         *
         * @note Requires a valid OpenGL context and initialized renderer.
         */
        void render(
            const helios::rendering::text::TextRenderCommand& command,
            const helios::rendering::shader::UniformValueMap& frameUniformValues)  noexcept override {

            if (auto* textPrototype = command.textRenderPrototype()) {

                const auto& baseShader = textPrototype->shader();

                const auto* shader = static_cast<const helios::ext::opengl::rendering::shader::OpenGLShader*>(&baseShader);
                assert(shader && "Unexpected failure when casting to OpenGLShader.");

                shader->use();

                shader->applyUniformValues(frameUniformValues);
                shader->applyUniformValues(command.textUniformValues());;

                const auto& drawProperties = command.drawProperties();
                const std::string_view text = command.text();
                const auto screenPosition = drawProperties.position;
                const auto scale = drawProperties.scale;
                const auto fontId = drawProperties.fontId;

                renderText(text, screenPosition, scale, fontId);
            }

        };




    };
}