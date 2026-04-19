/**
 * @file ViewportId.ixx
 * @brief Strongly-typed identifier for viewport resources.
 */
module;

export module helios.rendering.viewport.types.ViewportId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::rendering::viewport::types {

    /**
     * @brief Tag type for ViewportId.
     */
    struct ViewportIdTag{};

    /**
     * @brief Strongly-typed identifier for viewport resources.
     */
    using ViewportId = helios::core::types::StrongId<ViewportIdTag>;

}
