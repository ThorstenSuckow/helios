/**
 * @file MeshEntity.ixx
 * @brief Entity alias for mesh-managed ECS entities.
 */
module;

export module helios.rendering.mesh.MeshEntity;

import helios.rendering.mesh.MeshEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::rendering::mesh {

    /** @brief ECS entity alias bound to `MeshEntityManager`. */
    using MeshEntity = Entity<MeshEntityManager>;


}