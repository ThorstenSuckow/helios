/**
 * @file MaterialEntityManager.ixx
 * @brief Material-domain ECS registry and manager aliases.
 */
module;

#include "helios/helios_config.h"

export module helios.rendering.material.MaterialEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.rendering.material.types;

constexpr auto MATERIAL_SIZE = DEFAULT_MATERIAL_POOL_CAPACITY;

using namespace helios::rendering::material::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::rendering::material {

    /**
     * @brief Entity registry used for material resources.
     */
    using MaterialEntityRegistry = EntityRegistry<MaterialDomainTag, HashedLookupStrategy, true, MATERIAL_SIZE>;

    /**
     * @brief Entity manager used for material handles and material components.
     */
    using MaterialEntityManager = EntityManager<MaterialHandle, MaterialEntityRegistry, MATERIAL_SIZE>;


}