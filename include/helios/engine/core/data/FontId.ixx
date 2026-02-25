/**
 * @file FontId.ixx
 * @brief Strongly-typed identifier for font families.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.FontId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::core::data {

    /**
     * @brief Tag type for FontId.
     */
    struct FontIdTag{};

    /**
     * @brief Strongly-typed identifier for font families used in text rendering.
     *
     * @details Uses FNV-1a hashing to convert font family names to unique
     * 32-bit identifiers at compile time.
     *
     * @see helios::core::data::StrongId
     */
    using FontId = helios::core::data::StrongId<FontIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for FontId.
 */
template<>
struct std::hash<helios::engine::core::data::FontId> {
    std::size_t operator()(const helios::engine::core::data::FontId& id) const noexcept {
        return id.value();
    }

};
