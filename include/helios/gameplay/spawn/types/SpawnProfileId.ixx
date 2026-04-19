/**
 * @file SpawnProfileId.ixx
 * @brief Strongly-typed identifier for SpawnProfile instances.
 */
module;

#include <cstdint>
#include <string_view>
#include <cstddef>

export module helios.gameplay.spawn.types.SpawnProfileId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.core.types.StrongId;

export namespace helios::gameplay::spawn::types {

    /**
     * @brief Tag type for SpawnProfileId.
     */
    struct SpawnProfileIdTag{};

    /**
     * @brief Strongly-typed identifier for SpawnProfile instances.
     *
     * @details Provides type-safe identification for spawn profiles,
     * which define how entities are spawned (pool reference, placer,
     * initializer). Uses FNV-1a hashing for compile-time string-based
     * construction.
     *
     * @see helios::core::types::StrongId
     * @see SpawnRuleId
     * @see EntityPoolId
     */
    using SpawnProfileId = helios::core::types::StrongId<SpawnProfileIdTag>;

}

/**
 * @brief Hash specialization for SpawnProfileId.
 */
template<>
struct std::hash<helios::gameplay::spawn::types::SpawnProfileId> {
    std::size_t operator()(const helios::gameplay::spawn::types::SpawnProfileId& id) const noexcept {
        return id.value();
    }

};
