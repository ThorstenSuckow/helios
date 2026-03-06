/** @file GameObjectPoolId.ixx
 * @brief Strongly-typed identifier for GameObjectPool instances.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.runtime.pooling.types.GameObjectPoolId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::runtime::pooling::types {

    /**
     * @brief Tag type for GameObjectPoolId.
     */
    struct GameObjectPoolIdTag{};

    /**
     * @brief Strongly-typed identifier for referencing a GameObjectPool.
     *
     * @details Provides type-safety when working with multiple object pools.
     * Uses FNV-1a hashing for compile-time string-based construction.
     *
     * @see helios::core::data::StrongId
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    using GameObjectPoolId = helios::core::data::StrongId<GameObjectPoolIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for GameObjectPoolId.
 */
template<>
struct std::hash<helios::engine::runtime::pooling::types::GameObjectPoolId> {
    std::size_t operator()(const helios::engine::runtime::pooling::types::GameObjectPoolId& id) const noexcept {
        return id.value();
    }
};
