/**
* @file MenuId.ixx
 * @brief Strongly-typed identifier for UI menus.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.ui.widgets.types.MenuId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::ui::widgets::types {

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
     * @see helios::core::types::StrongId
     */
    using MenuId = helios::core::types::StrongId<MenuIdTag>;

}
