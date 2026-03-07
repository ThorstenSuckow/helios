/**
 * @file SceneId.ixx
 * @brief Strongly-typed identifier for scenes.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.modules.scene.types.SceneId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::modules::scene::types {

    /**
     * @brief Tag type for SceneId.
     */
    struct SceneIdTag{};

    /**
     * @brief Strongly-typed identifier for scenes.
     *
     * @details Uses FNV-1a string hashing to generate unique identifiers
     * at compile time. Enables type-safe referencing of scenes by name
     * (e.g., "Ui", "Game").
     *
     * @see helios::core::data::StrongId
     */
    using SceneId = helios::core::data::StrongId<SceneIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for SceneId.
 */
template<>
struct std::hash<helios::engine::modules::scene::types::SceneId> {
    std::size_t operator()(const helios::engine::modules::scene::types::SceneId& id) const noexcept {
        return id.value();
    }

};
