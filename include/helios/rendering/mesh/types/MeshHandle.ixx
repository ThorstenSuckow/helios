/**
 * @file MeshHandle.ixx
 * @brief Entity-handle alias for mesh resources.
 */
module;

export module helios.rendering.mesh.types.MeshHandle;


import helios.ecs;
import helios.rendering.mesh.types.MeshId;

using namespace helios::ecs;
export namespace helios::rendering::mesh::types {

    /**
     * @brief Handle type used to reference mesh entities.
     */
    using MeshHandle = helios::ecs::types::EntityHandle<MeshId>;

}
