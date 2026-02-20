/**
* @file PrefabId.ixx
 * @brief Strongly-typed identifier for UI menus.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.PrefabId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::core::data {

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
