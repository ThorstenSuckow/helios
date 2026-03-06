/**
 * @file PrefabId.ixx
 * @brief Strongly-typed identifier for prefab templates.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.common.types.PrefabId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::common::types {

    /**
     * @brief Tag type for PrefabId.
     */
    struct PrefabIdTag{};

    /**
     * @brief Strongly-typed identifier for prefabs.
     *
     * @details Used to uniquely identify prefabs.
     *
     * @see helios::core::data::StrongId
     */
    using PrefabId = helios::core::data::StrongId<PrefabIdTag, uint32_t>;

}
