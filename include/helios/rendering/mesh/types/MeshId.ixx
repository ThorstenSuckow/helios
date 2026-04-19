/**
 * @file MeshId.ixx
 * @brief Strongly-typed identifier for mesh resources.
 */
module;

export module helios.rendering.mesh.types.MeshId;

import helios.core.types.FuncDefs;
import helios.core.types;
import helios.ecs.types;

export namespace helios::rendering::mesh::types {

    /**
     * @brief Tag type for MeshId.
     */
    struct MeshIdTag{};

    /**
     * @brief Strongly-typed identifier for mesh resources.
     */
    using MeshId = helios::core::types::StrongId<MeshIdTag>;

}
