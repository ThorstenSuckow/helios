/**
 * @file ViewportId.ixx
 * @brief Strongly-typed identifier for viewports.
 */
module;

#include <cstdint>
#include <string_view>

export module helios.engine.core.data.ViewportId;

import helios.core.algorithms;
import helios.core.types;
import helios.core.data;

export namespace helios::engine::core::data {

    /**
     * @brief Tag type for ViewportId.
     */
    struct ViewportIdTag{};

    /**
     * @brief Strongly-typed identifier for viewports.
     *
     * @details Used to uniquely identify viewports across the rendering
     * system. Supports compile-time string-based construction via FNV-1a
     * hashing.
     *
     * @see helios::core::data::StrongId
     */
    using ViewportId = helios::core::data::StrongId<ViewportIdTag, uint32_t>;

}
