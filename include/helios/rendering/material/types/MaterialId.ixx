/**
 * @file MaterialId.ixx
 * @brief Strongly-typed identifier for material resources.
 */
module;

export module helios.rendering.material.types.MaterialId;

import helios.core.types.StrongId;
import helios.rendering.material.types.MaterialDomainTag;

export namespace helios::rendering::material::types {

    /**
     * @brief Strongly-typed identifier for material resources.
     */
    using MaterialId = helios::core::types::StrongId<MaterialDomainTag>;

}
