/**
 * @file ViewportId.ixx
 * @brief Strongly-typed identifier for viewport resources.
 */
module;

export module helios.rendering.viewport.types.ViewportId;

import helios.core.types.StrongId;
import helios.rendering.viewport.types.ViewportDomainTag;

export namespace helios::rendering::viewport::types {

    /**
     * @brief Strongly-typed identifier for viewport resources.
     */
    using ViewportId = helios::core::types::StrongId<ViewportDomainTag>;

}
