/**
 * @file ActionId.ixx
 * @brief Strongly-typed identifier for UI actions.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.ActionId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::core::data {

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
     * @see helios::core::data::StrongId
     */
    using ActionId = helios::core::data::StrongId<ActionIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for ActionId.
 */
template<>
struct std::hash<helios::engine::core::data::ActionId> {
    std::size_t operator()(const helios::engine::core::data::ActionId& id) const noexcept {
        return id.value();
    }

};
