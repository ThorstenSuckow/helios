/**
 * @file MeshId.ixx
 * @brief Strongly-typed identifier for mesh resources.
 */
module;

export module helios.rendering.mesh.types.MeshId;

import helios.core.types.StrongId;
import helios.rendering.mesh.types.MeshDomainTag;

export namespace helios::rendering::mesh::types {

    /**
     * @brief Strongly-typed identifier for mesh resources.
     */
    using MeshId = helios::core::types::StrongId<MeshDomainTag>;

}
