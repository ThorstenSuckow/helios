/**
 * @file SpawnProfileId.ixx
 * @brief Strongly-typed identifier for SpawnProfile instances.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.mechanics.spawn.types.SpawnProfileId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::mechanics::spawn::types {

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
     * @see helios::core::data::StrongId
     * @see SpawnRuleId
     * @see GameObjectPoolId
     */
    using SpawnProfileId = helios::core::data::StrongId<SpawnProfileIdTag, uint32_t>;

}

/**
 * @brief Hash specialization for SpawnProfileId.
 */
template<>
struct std::hash<helios::engine::mechanics::spawn::types::SpawnProfileId> {
    std::size_t operator()(const helios::engine::mechanics::spawn::types::SpawnProfileId& id) const noexcept {
        return id.value();
    }

};
