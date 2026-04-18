/** @file GameObjectPoolId.ixx
 * @brief Strongly-typed identifier for GameObjectPool instances.
 */
module;

#include <cstdint>
#include <string_view>
#include <cstddef>

export module helios.runtime.pooling.types.GameObjectPoolId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::runtime::pooling::types {

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
     * @see helios::core::types::StrongId
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    using GameObjectPoolId = helios::core::types::StrongId<GameObjectPoolIdTag>;

}

/**
 * @brief Hash specialization for GameObjectPoolId.
 */
template<>
struct std::hash<helios::runtime::pooling::types::GameObjectPoolId> {
    std::size_t operator()(const helios::runtime::pooling::types::GameObjectPoolId& id) const noexcept {
        return id.value();
    }
};
