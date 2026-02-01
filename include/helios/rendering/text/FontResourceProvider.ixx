/**
 * @file FontResourceProvider.ixx
 * @brief Abstract interface for loading and providing font glyph data.
 */
module;

#include <string_view>

export module helios.rendering.text.FontResourceProvider;

import helios.rendering.text.Glyph;

import helios.engine.core.data.FontId;


export namespace helios::rendering::text{

    /**
     * @brief Abstract interface for loading fonts and providing glyph data.
     *
     * `FontResourceProvider` defines the contract for font management in the helios
     * text rendering system. Implementations handle the specifics of loading font files
     * and caching glyph data for efficient text rendering.
     *
     * ## Responsibilities
     *
     * - **Font Loading:** Load font files from disk and parse glyph metrics.
     * - **Glyph Retrieval:** Provide glyph data (texture, metrics) for individual characters.
     * - **Caching:** Cache loaded glyphs to avoid redundant loading.
     *
     * @see Glyph
     * @see TextMesh
     * @see OpenGLGlyphTextRenderer
     */
    class FontResourceProvider {


    public:

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~FontResourceProvider() = default;

        /**
         * @brief Loads a font from the specified file path.
         *
         * Parses the font file and caches glyph data for later retrieval.
         * The font is associated with the given `FontId` for identification.
         *
         * @param fontId Unique identifier for this font.
         * @param pathToFont File system path to the font file (e.g., TrueType `.ttf`).
         *
         * @note If a font with the same `fontId` is already loaded, behavior is
         *       implementation-defined (may replace or ignore).
         */
        virtual void loadFont(helios::engine::core::data::FontId fontId, std::string_view pathToFont) = 0;

        /**
         * @brief Retrieves the glyph data for a character.
         *
         * Returns the cached glyph metrics and texture ID for the specified character
         * and font. The font must have been previously loaded via `loadFont()`.
         *
         * @param c The character to retrieve the glyph for.
         * @param fontId The font to use for rendering.
         *
         * @return The glyph data for the character.
         *
         * @note This method is marked `noexcept` and must not throw exceptions.
         *       If the character is not available, a fallback glyph should be returned.
         */
        virtual helios::rendering::text::Glyph glyph(char c, helios::engine::core::data::FontId fontId) = 0;



    };


}