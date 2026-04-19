/**
 * @file SpawnRuleId.ixx
 * @brief Strongly-typed identifier for SpawnRule instances.
 */
module;

#include <cstdint>
#include <string_view>
#include <cstddef>

export module helios.gameplay.spawn.types.SpawnRuleId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::gameplay::spawn::types {

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
     * @see helios::core::types::StrongId
     * @see SpawnProfileId
     * @see EntityPoolId
     */
    using SpawnRuleId = helios::core::types::StrongId<SpawnRuleIdTag>;

}

/**
 * @brief Hash specialization for SpawnRuleId.
 */
template<>
struct std::hash<helios::gameplay::spawn::types::SpawnRuleId> {
    std::size_t operator()(const helios::gameplay::spawn::types::SpawnRuleId& id) const noexcept {
        return id.value();
    }

};
