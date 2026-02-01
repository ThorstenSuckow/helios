/**
 * @file FreeTypeFontResourceManager.ixx
 * @brief FreeType-based font loading and glyph caching for OpenGL text rendering.
 */
module;

#include <string_view>

#include <cassert>
#include <ft2build.h>
#include <glad/gl.h>

#include <ostream>
#include <ranges>
#include <unordered_map>


#include FT_FREETYPE_H

export module helios.ext.opengl.rendering.FreeTypeFontResourceManager;

import helios.rendering.text.FontResourceProvider;
import helios.rendering.text.Glyph;

import helios.engine.core.data.FontId;


export namespace helios::ext::opengl::rendering {

    /**
     * @brief FreeType-based implementation of `FontResourceProvider` for OpenGL.
     *
     * `FreeTypeFontResourceManager` uses the FreeType library to load TrueType fonts,
     * rasterize glyphs, and cache them as OpenGL textures. Each glyph is stored as a
     * separate texture for simplicity.
     *
     * ## Responsibilities
     *
     * - **Font Loading:** Parse font files using FreeType and extract glyph metrics.
     * - **Texture Creation:** Upload glyph bitmaps to GPU as GL_R8 textures.
     * - **Caching:** Store glyph data per font for efficient repeated access.
     * - **Resource Cleanup:** Delete OpenGL textures when the manager is destroyed.
     *
     * ## Usage
     *
     * ```cpp
     * FreeTypeFontResourceManager fontManager;
     * fontManager.loadFont(FontId{"roboto"}, "fonts/Roboto-Regular.ttf");
     *
     * // Retrieve a glyph for rendering
     * Glyph g = fontManager.glyph('A', FontId{"roboto"});
     * ```
     *
     * @note Currently loads ASCII characters 0-127 with a fixed pixel size of 24.
     *
     * @see FontResourceProvider
     * @see Glyph
     * @see OpenGLGlyphTextRenderer
     */
    class FreeTypeFontResourceManager : public helios::rendering::text::FontResourceProvider {

        /**
         * @brief Internal cache structure for a single font's glyphs.
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


    public:

        /**
         * @brief Destructor that cleans up all OpenGL textures.
         *
         * Iterates through all cached fonts and deletes the associated glyph textures
         * to free GPU memory.
         */
        ~FreeTypeFontResourceManager() override {

            // Cleanup texture memory for all loaded fonts
            for (const auto& cache: fontCache_ | std::views::values) {
                for (const auto& glyph : cache.characters) {
                    // glDeleteTextures ignores 0, so this is safe even if textureId is uninitialized (0)
                    glDeleteTextures(1, &glyph.textureId);
                }
            }
        }

        /**
         * @brief Loads a font from a file and caches its glyphs as OpenGL textures.
         *
         * Uses FreeType to parse the font file and rasterize ASCII characters 0-127.
         * Each glyph bitmap is uploaded to the GPU as a GL_R8 texture.
         *
         * @param fontId Unique identifier for this font.
         * @param pathToFont File system path to the font file (e.g., `.ttf`).
         *
         * @throws std::runtime_error If FreeType initialization fails.
         * @throws std::runtime_error If the font file cannot be loaded.
         * @throws std::runtime_error If a glyph cannot be loaded.
         *
         * @pre The font ID must not already be loaded (asserts in debug builds).
         */
        void loadFont(
            helios::engine::core::data::FontId fontId,
            std::string_view pathToFont
        ) override {

            const int begin = 0;
            const int end = 128;

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

            FT_Set_Pixel_Sizes(face, 0, 24);
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
        };

        /**
         * @brief Retrieves the cached glyph data for a character.
         *
         * Returns the glyph metrics and texture ID for the specified character
         * from the font's cache. The font must have been previously loaded via `loadFont()`.
         *
         * @param c The character to retrieve the glyph for.
         * @param fontId The font identifier to use.
         *
         * @return The cached `Glyph` data for the character.
         *
         * @pre The font must already be loaded (asserts in debug builds).
         */
        helios::rendering::text::Glyph glyph(
            const char c,
            const helios::engine::core::data::FontId fontId
        ) override {

            assert(fontCache_.contains(fontId) && "FontCache already existing");

            return fontCache_[fontId].characters[c];
        }



    };


}