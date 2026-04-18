/**
 * @file ActionId.ixx
 * @brief Strongly-typed identifier for UI actions.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.modules.ui.widgets.types.ActionId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::engine::modules::ui::widgets::types {

    /**
     * @brief Tag type for ActionId.
     */
    struct ActionIdTag{};

    /**
     * @brief Strongly-typed identifier for UI actions.
     *
     * @details Uses FNV-1a hashing to convert string literals to unique IDs
     * at compile time. Supports comparison and hashing for use in containers.
     *
     * @see helios::core::types::StrongId
     */
    using ActionId = helios::core::types::StrongId<ActionIdTag>;

}

/**
 * @brief Hash specialization for ActionId.
 */
template<>
struct std::hash<helios::engine::modules::ui::widgets::types::ActionId> {
    std::size_t operator()(const helios::engine::modules::ui::widgets::types::ActionId& id) const noexcept {
        return id.value();
    }

};
