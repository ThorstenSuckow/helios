/**
* @file MenuId.ixx
 * @brief Strongly-typed identifier for UI menus.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.MenuId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::core::data {

    /**
     * @brief Tag type for MenuId.
     */
    struct MenuIdTag{};

    /**
     * @brief Strongly-typed identifier for menus.
     *
     * @details Used to uniquely identify menus across the rendering
     * system. Supports compile-time string-based construction via FNV-1a
     * hashing.
     *
     * @see helios::core::data::StrongId
     */
    using MenuId = helios::core::data::StrongId<MenuIdTag, uint32_t>;

}
