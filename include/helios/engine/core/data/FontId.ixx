/**
 * @file FontId.ixx
 * @brief Strongly-typed identifier for font families.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.FontId;

import helios.core.algorithms;

export namespace helios::engine::core::data {

    /**
     * @brief Strongly-typed identifier for font families used in text rendering.
     *
     * `FontId` provides a compile-time or runtime identifier for font families
     * registered with the `TextRenderer`. It uses FNV-1a hashing to convert
     * string names to unique 32-bit identifiers.
     *
     * ## Usage
     *
     * ```cpp
     * // Compile-time construction from string literal
     * constexpr FontId arialFont("Arial");
     * constexpr FontId monoFont("Monospace");
     *
     * // Register font with TextRenderer
     * textRenderer.addFontFamily(arialFont, "fonts/arial.ttf");
     *
     * // Use in TextMesh
     * TextMesh props{arialFont, {100.0f, 200.0f}, 1.0f};
     * ```
     *
     * @see TextRenderer::addFontFamily()
     * @see TextMesh
     */
    struct FontId {

    private:

        /**
         * @brief The hashed identifier value.
         */
        uint32_t id_;

        /**
         * @brief Private constructor from raw hash value.
         *
         * @param id The pre-computed hash value.
         */
        explicit constexpr FontId(uint32_t id) noexcept
            : id_(id) {
        }

    public:

        /**
         * @brief Constructs a FontId from a string name.
         *
         * Uses FNV-1a hashing to convert the string to a 32-bit identifier.
         * This constructor is `constexpr`, allowing compile-time evaluation.
         *
         * @param str The font family name (e.g., "Arial", "Monospace").
         */
        explicit constexpr FontId(const std::string_view str) noexcept
            : FontId(helios::core::algorithms::fnv1a_hash(str)) {}

        /**
         * @brief Returns the raw identifier value.
         *
         * @return The 32-bit hash value.
         */
        [[nodiscard]] uint32_t value() const noexcept {
            return id_;
        }

        /**
         * @brief Equality comparison operator.
         */
        friend constexpr bool operator==(FontId, FontId) noexcept = default;

        /**
         * @brief Less-than comparison for ordered containers.
         *
         * @param other The FontId to compare against.
         *
         * @return `true` if this id is less than `other`.
         */
        constexpr bool operator<(const FontId& other) const noexcept {
            return id_ < other.id_;
        }

        /**
         * @brief Greater-than comparison for ordered containers.
         *
         * @param other The FontId to compare against.
         *
         * @return `true` if this id is greater than `other`.
         */
        constexpr bool operator>(const FontId& other) const noexcept {
            return id_ > other.id_;
        }

    };

}

/**
 * @brief Hash specialization for `FontId` to enable use in unordered containers.
 */
template<>
struct std::hash<helios::engine::core::data::FontId> {

    /**
     * @brief Computes the hash value for a FontId.
     *
     * @param id The FontId to hash.
     *
     * @return The hash value (same as the internal id).
     */
    std::size_t operator()(const helios::engine::core::data::FontId& id) const noexcept {
        return id.value();
    }

};