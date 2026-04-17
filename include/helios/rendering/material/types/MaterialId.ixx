/**
 * @file MaterialId.ixx
 * @brief Strongly-typed identifier for material resources.
 */
module;

export module helios.rendering.material.types.MaterialId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::rendering::material::types {

    /**
     * @brief Tag type for MaterialId.
     */
    struct MaterialIdTag{};

    /**
     * @brief Strongly-typed identifier for material resources.
     */
    using MaterialId = helios::core::types::StrongId<MaterialIdTag>;

}
