/**
 * @file Colors.ixx
 * @brief Standard color constants as vec4f (RGBA).
 */
export module helios.util.Colors;

import helios.math.types;

export namespace helios::util {

    /**
     * @brief Standard color palette providing commonly used colors as vec4f (RGBA).
     *
     * All colors are defined in normalized RGBA format with components in [0.0, 1.0].
     * Alpha is set to 1.0 (fully opaque) by default.
     */
    struct Colors {

        // =====================================================================
        // Basic Colors
        // =====================================================================

        /** @brief Pure black (#000000) */
        static constexpr helios::math::vec4f Black{0.0f, 0.0f, 0.0f, 1.0f};

        /** @brief Pure white (#FFFFFF) */
        static constexpr helios::math::vec4f White{1.0f, 1.0f, 1.0f, 1.0f};

        /** @brief Pure red (#FF0000) */
        static constexpr helios::math::vec4f Red{1.0f, 0.0f, 0.0f, 1.0f};

        /** @brief Pure green (#00FF00) */
        static constexpr helios::math::vec4f Green{0.0f, 1.0f, 0.0f, 1.0f};

        /** @brief Pure blue (#0000FF) */
        static constexpr helios::math::vec4f Blue{0.0f, 0.0f, 1.0f, 1.0f};

        /** @brief Yellow (#FFFF00) */
        static constexpr helios::math::vec4f Yellow{1.0f, 1.0f, 0.0f, 1.0f};

        /** @brief Cyan / Aqua (#00FFFF) */
        static constexpr helios::math::vec4f Cyan{0.0f, 1.0f, 1.0f, 1.0f};

        /** @brief Magenta / Fuchsia (#FF00FF) */
        static constexpr helios::math::vec4f Magenta{1.0f, 0.0f, 1.0f, 1.0f};

        // =====================================================================
        // Grayscale
        // =====================================================================

        /** @brief Light gray (#C0C0C0) */
        static constexpr helios::math::vec4f LightGray{0.75f, 0.75f, 0.75f, 1.0f};

        /** @brief Gray (#808080) */
        static constexpr helios::math::vec4f Gray{0.5f, 0.5f, 0.5f, 1.0f};

        /** @brief Dark gray (#404040) */
        static constexpr helios::math::vec4f DarkGray{0.25f, 0.25f, 0.25f, 1.0f};

        // =====================================================================
        // Reds / Warm Colors
        // =====================================================================

        /** @brief Dark red (#8B0000) */
        static constexpr helios::math::vec4f DarkRed{0.545f, 0.0f, 0.0f, 1.0f};

        /** @brief Crimson (#DC143C) */
        static constexpr helios::math::vec4f Crimson{0.863f, 0.078f, 0.235f, 1.0f};

        /** @brief Firebrick (#B22222) */
        static constexpr helios::math::vec4f Firebrick{0.698f, 0.133f, 0.133f, 1.0f};

        /** @brief Tomato (#FF6347) */
        static constexpr helios::math::vec4f Tomato{1.0f, 0.388f, 0.278f, 1.0f};

        /** @brief Coral (#FF7F50) */
        static constexpr helios::math::vec4f Coral{1.0f, 0.498f, 0.314f, 1.0f};

        /** @brief Indian Red (#CD5C5C) */
        static constexpr helios::math::vec4f IndianRed{0.804f, 0.361f, 0.361f, 1.0f};

        /** @brief Salmon (#FA8072) */
        static constexpr helios::math::vec4f Salmon{0.980f, 0.502f, 0.447f, 1.0f};

        /** @brief Light Salmon (#FFA07A) */
        static constexpr helios::math::vec4f LightSalmon{1.0f, 0.627f, 0.478f, 1.0f};

        // =====================================================================
        // Oranges
        // =====================================================================

        /** @brief Orange (#FFA500) */
        static constexpr helios::math::vec4f Orange{1.0f, 0.647f, 0.0f, 1.0f};

        /** @brief Dark Orange (#FF8C00) */
        static constexpr helios::math::vec4f DarkOrange{1.0f, 0.549f, 0.0f, 1.0f};

        /** @brief Orange Red (#FF4500) */
        static constexpr helios::math::vec4f OrangeRed{1.0f, 0.271f, 0.0f, 1.0f};

        // =====================================================================
        // Yellows
        // =====================================================================

        /** @brief Gold (#FFD700) */
        static constexpr helios::math::vec4f Gold{1.0f, 0.843f, 0.0f, 1.0f};

        /** @brief Light Yellow (#FFFFE0) */
        static constexpr helios::math::vec4f LightYellow{1.0f, 1.0f, 0.878f, 1.0f};

        /** @brief Lemon Chiffon (#FFFACD) */
        static constexpr helios::math::vec4f LemonChiffon{1.0f, 0.980f, 0.804f, 1.0f};

        /** @brief Khaki (#F0E68C) */
        static constexpr helios::math::vec4f Khaki{0.941f, 0.902f, 0.549f, 1.0f};

        // =====================================================================
        // Greens
        // =====================================================================

        /** @brief Dark Green (#006400) */
        static constexpr helios::math::vec4f DarkGreen{0.0f, 0.392f, 0.0f, 1.0f};

        /** @brief Forest Green (#228B22) */
        static constexpr helios::math::vec4f ForestGreen{0.133f, 0.545f, 0.133f, 1.0f};

        /** @brief Lime Green (#32CD32) */
        static constexpr helios::math::vec4f LimeGreen{0.196f, 0.804f, 0.196f, 1.0f};

        /** @brief Light Green (#90EE90) */
        static constexpr helios::math::vec4f LightGreen{0.565f, 0.933f, 0.565f, 1.0f};

        /** @brief Pale Green (#98FB98) */
        static constexpr helios::math::vec4f PaleGreen{0.596f, 0.984f, 0.596f, 1.0f};

        /** @brief Spring Green (#00FF7F) */
        static constexpr helios::math::vec4f SpringGreen{0.0f, 1.0f, 0.498f, 1.0f};

        /** @brief Sea Green (#2E8B57) */
        static constexpr helios::math::vec4f SeaGreen{0.180f, 0.545f, 0.341f, 1.0f};

        /** @brief Olive (#808000) */
        static constexpr helios::math::vec4f Olive{0.502f, 0.502f, 0.0f, 1.0f};

        /** @brief Olive Drab (#6B8E23) */
        static constexpr helios::math::vec4f OliveDrab{0.420f, 0.557f, 0.137f, 1.0f};

        // =====================================================================
        // Blues
        // =====================================================================

        /** @brief Navy (#000080) */
        static constexpr helios::math::vec4f Navy{0.0f, 0.0f, 0.502f, 1.0f};

        /** @brief Dark Blue (#00008B) */
        static constexpr helios::math::vec4f DarkBlue{0.0f, 0.0f, 0.545f, 1.0f};

        /** @brief Medium Blue (#0000CD) */
        static constexpr helios::math::vec4f MediumBlue{0.0f, 0.0f, 0.804f, 1.0f};

        /** @brief Royal Blue (#4169E1) */
        static constexpr helios::math::vec4f RoyalBlue{0.255f, 0.412f, 0.882f, 1.0f};

        /** @brief Cornflower Blue (#6495ED) */
        static constexpr helios::math::vec4f CornflowerBlue{0.392f, 0.584f, 0.929f, 1.0f};

        /** @brief Dodger Blue (#1E90FF) */
        static constexpr helios::math::vec4f DodgerBlue{0.118f, 0.565f, 1.0f, 1.0f};

        /** @brief Deep Sky Blue (#00BFFF) */
        static constexpr helios::math::vec4f DeepSkyBlue{0.0f, 0.749f, 1.0f, 1.0f};

        /** @brief Sky Blue (#87CEEB) */
        static constexpr helios::math::vec4f SkyBlue{0.529f, 0.808f, 0.922f, 1.0f};

        /** @brief Light Sky Blue (#87CEFA) */
        static constexpr helios::math::vec4f LightSkyBlue{0.529f, 0.808f, 0.980f, 1.0f};

        /** @brief Light Blue (#ADD8E6) */
        static constexpr helios::math::vec4f LightBlue{0.678f, 0.847f, 0.902f, 1.0f};

        /** @brief Steel Blue (#4682B4) */
        static constexpr helios::math::vec4f SteelBlue{0.275f, 0.510f, 0.706f, 1.0f};

        /** @brief Powder Blue (#B0E0E6) */
        static constexpr helios::math::vec4f PowderBlue{0.690f, 0.878f, 0.902f, 1.0f};

        // =====================================================================
        // Cyans / Teals
        // =====================================================================

        /** @brief Teal (#008080) */
        static constexpr helios::math::vec4f Teal{0.0f, 0.502f, 0.502f, 1.0f};

        /** @brief Dark Cyan (#008B8B) */
        static constexpr helios::math::vec4f DarkCyan{0.0f, 0.545f, 0.545f, 1.0f};

        /** @brief Turquoise (#40E0D0) */
        static constexpr helios::math::vec4f Turquoise{0.251f, 0.878f, 0.816f, 1.0f};

        /** @brief Aquamarine (#7FFFD4) */
        static constexpr helios::math::vec4f Aquamarine{0.498f, 1.0f, 0.831f, 1.0f};

        // =====================================================================
        // Purples / Violets
        // =====================================================================

        /** @brief Purple (#800080) */
        static constexpr helios::math::vec4f Purple{0.502f, 0.0f, 0.502f, 1.0f};

        /** @brief Indigo (#4B0082) */
        static constexpr helios::math::vec4f Indigo{0.294f, 0.0f, 0.510f, 1.0f};

        /** @brief Dark Violet (#9400D3) */
        static constexpr helios::math::vec4f DarkViolet{0.580f, 0.0f, 0.827f, 1.0f};

        /** @brief Dark Orchid (#9932CC) */
        static constexpr helios::math::vec4f DarkOrchid{0.600f, 0.196f, 0.800f, 1.0f};

        /** @brief Medium Purple (#9370DB) */
        static constexpr helios::math::vec4f MediumPurple{0.576f, 0.439f, 0.859f, 1.0f};

        /** @brief Violet (#EE82EE) */
        static constexpr helios::math::vec4f Violet{0.933f, 0.510f, 0.933f, 1.0f};

        /** @brief Orchid (#DA70D6) */
        static constexpr helios::math::vec4f Orchid{0.855f, 0.439f, 0.839f, 1.0f};

        /** @brief Plum (#DDA0DD) */
        static constexpr helios::math::vec4f Plum{0.867f, 0.627f, 0.867f, 1.0f};

        /** @brief Lavender (#E6E6FA) */
        static constexpr helios::math::vec4f Lavender{0.902f, 0.902f, 0.980f, 1.0f};

        // =====================================================================
        // Pinks
        // =====================================================================

        /** @brief Pink (#FFC0CB) */
        static constexpr helios::math::vec4f Pink{1.0f, 0.753f, 0.796f, 1.0f};

        /** @brief Light Pink (#FFB6C1) */
        static constexpr helios::math::vec4f LightPink{1.0f, 0.714f, 0.757f, 1.0f};

        /** @brief Hot Pink (#FF69B4) */
        static constexpr helios::math::vec4f HotPink{1.0f, 0.412f, 0.706f, 1.0f};

        /** @brief Deep Pink (#FF1493) */
        static constexpr helios::math::vec4f DeepPink{1.0f, 0.078f, 0.576f, 1.0f};

        // =====================================================================
        // Browns
        // =====================================================================

        /** @brief Brown (#A52A2A) */
        static constexpr helios::math::vec4f Brown{0.647f, 0.165f, 0.165f, 1.0f};

        /** @brief Maroon (#800000) */
        static constexpr helios::math::vec4f Maroon{0.502f, 0.0f, 0.0f, 1.0f};

        /** @brief Sienna (#A0522D) */
        static constexpr helios::math::vec4f Sienna{0.627f, 0.322f, 0.176f, 1.0f};

        /** @brief Saddle Brown (#8B4513) */
        static constexpr helios::math::vec4f SaddleBrown{0.545f, 0.271f, 0.075f, 1.0f};

        /** @brief Chocolate (#D2691E) */
        static constexpr helios::math::vec4f Chocolate{0.824f, 0.412f, 0.118f, 1.0f};

        /** @brief Peru (#CD853F) */
        static constexpr helios::math::vec4f Peru{0.804f, 0.522f, 0.247f, 1.0f};

        /** @brief Sandy Brown (#F4A460) */
        static constexpr helios::math::vec4f SandyBrown{0.957f, 0.643f, 0.376f, 1.0f};

        /** @brief Tan (#D2B48C) */
        static constexpr helios::math::vec4f Tan{0.824f, 0.706f, 0.549f, 1.0f};

        /** @brief Burlywood (#DEB887) */
        static constexpr helios::math::vec4f Burlywood{0.871f, 0.722f, 0.529f, 1.0f};

        /** @brief Wheat (#F5DEB3) */
        static constexpr helios::math::vec4f Wheat{0.961f, 0.871f, 0.702f, 1.0f};

        // =====================================================================
        // Whites / Light Colors
        // =====================================================================

        /** @brief Snow (#FFFAFA) */
        static constexpr helios::math::vec4f Snow{1.0f, 0.980f, 0.980f, 1.0f};

        /** @brief Ivory (#FFFFF0) */
        static constexpr helios::math::vec4f Ivory{1.0f, 1.0f, 0.941f, 1.0f};

        /** @brief Linen (#FAF0E6) */
        static constexpr helios::math::vec4f Linen{0.980f, 0.941f, 0.902f, 1.0f};

        /** @brief Beige (#F5F5DC) */
        static constexpr helios::math::vec4f Beige{0.961f, 0.961f, 0.863f, 1.0f};

        /** @brief Antique White (#FAEBD7) */
        static constexpr helios::math::vec4f AntiqueWhite{0.980f, 0.922f, 0.843f, 1.0f};

        /** @brief Mint Cream (#F5FFFA) */
        static constexpr helios::math::vec4f MintCream{0.961f, 1.0f, 0.980f, 1.0f};

        /** @brief Azure (#F0FFFF) */
        static constexpr helios::math::vec4f Azure{0.941f, 1.0f, 1.0f, 1.0f};

        /** @brief Alice Blue (#F0F8FF) */
        static constexpr helios::math::vec4f AliceBlue{0.941f, 0.973f, 1.0f, 1.0f};

        /** @brief Ghost White (#F8F8FF) */
        static constexpr helios::math::vec4f GhostWhite{0.973f, 0.973f, 1.0f, 1.0f};

        /** @brief White Smoke (#F5F5F5) */
        static constexpr helios::math::vec4f WhiteSmoke{0.961f, 0.961f, 0.961f, 1.0f};

        // =====================================================================
        // Special / Transparent
        // =====================================================================

        /** @brief Fully transparent (all components 0) */
        static constexpr helios::math::vec4f Transparent{0.0f, 0.0f, 0.0f, 0.0f};

        /** @brief Cornsilk (#FFF8DC) */
        static constexpr helios::math::vec4f Cornsilk{1.0f, 0.973f, 0.863f, 1.0f};

        /** @brief Honeydew (#F0FFF0) */
        static constexpr helios::math::vec4f Honeydew{0.941f, 1.0f, 0.941f, 1.0f};

        /** @brief Seashell (#FFF5EE) */
        static constexpr helios::math::vec4f Seashell{1.0f, 0.961f, 0.933f, 1.0f};

        /** @brief Floral White (#FFFAF0) */
        static constexpr helios::math::vec4f FloralWhite{1.0f, 0.980f, 0.941f, 1.0f};

        // =====================================================================
        // Utility Methods
        // =====================================================================

        /**
         * @brief Creates a color from RGB byte values (0-255).
         * @param r Red component (0-255).
         * @param g Green component (0-255).
         * @param b Blue component (0-255).
         * @param a Alpha component (0-255), defaults to 255 (opaque).
         * @return Normalized vec4f color.
         */
        static constexpr helios::math::vec4f fromRGB(int r, int g, int b, int a = 255) {
            return helios::math::vec4f{
                static_cast<float>(r) / 255.0f,
                static_cast<float>(g) / 255.0f,
                static_cast<float>(b) / 255.0f,
                static_cast<float>(a) / 255.0f
            };
        }

        /**
         * @brief Creates a color from a 32-bit hex value (0xRRGGBB or 0xRRGGBBAA).
         * @param hex The hex color value.
         * @param hasAlpha If true, expects 0xRRGGBBAA format; otherwise 0xRRGGBB with alpha = 1.0.
         * @return Normalized vec4f color.
         */
        static constexpr helios::math::vec4f fromHex(unsigned int hex, bool hasAlpha = false) {
            if (hasAlpha) {
                return helios::math::vec4f{
                    static_cast<float>((hex >> 24) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
                    static_cast<float>((hex >> 8) & 0xFF) / 255.0f,
                    static_cast<float>(hex & 0xFF) / 255.0f
                };
            }
            return helios::math::vec4f{
                static_cast<float>((hex >> 16) & 0xFF) / 255.0f,
                static_cast<float>((hex >> 8) & 0xFF) / 255.0f,
                static_cast<float>(hex & 0xFF) / 255.0f,
                1.0f
            };
        }

        /**
         * @brief Creates a grayscale color.
         * @param value The gray value (0.0 = black, 1.0 = white).
         * @param alpha The alpha value (defaults to 1.0).
         * @return Grayscale vec4f color.
         */
        static constexpr helios::math::vec4f grayscale(float value, float alpha = 1.0f) {
            return helios::math::vec4f{value, value, value, alpha};
        }

        /**
         * @brief Creates a color with modified alpha.
         * @param color The source color.
         * @param alpha The new alpha value.
         * @return Color with modified alpha.
         */
        static constexpr helios::math::vec4f withAlpha(const helios::math::vec4f& color, float alpha) {
            return helios::math::vec4f{color[0], color[1], color[2], alpha};
        }
    };

}

