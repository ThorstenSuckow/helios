/** @file EntityPoolId.ixx
 * @brief Strongly-typed identifier for EntityPool instances.
 */
module;

#include <cstdint>
#include <string_view>
#include <cstddef>

export module helios.runtime.pooling.types.EntityPoolId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::runtime::pooling::types {

    /**
     * @brief Tag type for EntityPoolId.
     */
    struct EntityPoolIdTag{};

    /**
     * @brief Strongly-typed identifier for referencing a EntityPool.
     *
     * @details Provides type-safety when working with multiple object pools.
     * Uses FNV-1a hashing for compile-time string-based construction.
     *
     * @see helios::core::types::StrongId
     * @see SpawnProfileId
     * @see SpawnRuleId
     */
    using EntityPoolId = helios::core::types::StrongId<EntityPoolIdTag>;

}

/**
 * @brief Hash specialization for EntityPoolId.
 */
template<>
struct std::hash<helios::runtime::pooling::types::EntityPoolId> {
    std::size_t operator()(const helios::runtime::pooling::types::EntityPoolId& id) const noexcept {
        return id.value();
    }
};
