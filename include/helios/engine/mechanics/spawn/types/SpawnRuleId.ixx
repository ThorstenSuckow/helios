/**
 * @file SpawnRuleId.ixx
 * @brief Strongly-typed identifier for SpawnRule instances.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.mechanics.spawn.types.SpawnRuleId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::mechanics::spawn::types {

    /**
     * @brief Tag type for SpawnRuleId.
     */
    struct SpawnRuleIdTag{};

    /**
     * @brief Strongly-typed identifier for SpawnRule instances.
     *
     * @details Provides type-safe identification for spawn rules,
     * which define the conditions under which entities are spawned
     * (timing, pool limits, etc.). Uses FNV-1a hashing for compile-time
     * string-based construction.
     *
     * @see helios::core::data::StrongId
     * @see SpawnProfileId
     * @see GameObjectPoolId
     */
    using SpawnRuleId = helios::core::data::StrongId<SpawnRuleIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for SpawnRuleId.
 */
template<>
struct std::hash<helios::engine::mechanics::spawn::types::SpawnRuleId> {
    std::size_t operator()(const helios::engine::mechanics::spawn::types::SpawnRuleId& id) const noexcept {
        return id.value();
    }

};
