/**
 * @file FontId.ixx
 * @brief Strongly-typed identifier for font families.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.ui.widgets.types.FontId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::ui::widgets::types {

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
     * @see helios::core::types::StrongId
     */
    using FontId = helios::core::types::StrongId<FontIdTag>;

}

/**
 * @brief Hash specialization for FontId.
 */
template<>
struct std::hash<helios::ui::widgets::types::FontId> {
    std::size_t operator()(const helios::ui::widgets::types::FontId& id) const noexcept {
        return id.value();
    }

};
