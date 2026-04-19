/**
 * @file MeshEntityManager.ixx
 * @brief Mesh-domain ECS registry and manager aliases.
 */
module;

#include "helios/helios_config.h"

export module helios.rendering.mesh.MeshEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.rendering.mesh.types;

constexpr auto MESH_SIZE = DEFAULT_MESH_POOL_CAPACITY;

using namespace helios::rendering::mesh::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::rendering::mesh {

    /**
     * @brief Entity registry used for mesh resources.
     */
    using MeshEntityRegistry = EntityRegistry<MeshId, HashedLookupStrategy, true, MESH_SIZE>;

    /**
     * @brief Entity manager used for mesh handles and mesh components.
     */
    using MeshEntityManager = EntityManager<MeshHandle, MeshEntityRegistry, MESH_SIZE>;


}