/**
 * @file SceneEntityManager.ixx
 * @brief Scene-domain ECS registry and manager aliases.
 */
module;

#include "helios/helios_config.h"

export module helios.scene.SceneEntityManager;

import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.strategies;

import helios.scene.types;

constexpr auto SCENE_SIZE = DEFAULT_SCENE_POOL_CAPACITY;

using namespace helios::scene::types;
using namespace helios::ecs;
using namespace helios::ecs::strategies;
export namespace helios::scene {

    /**
     * @brief Entity registry used for scene resources.
     */
    using SceneEntityRegistry = EntityRegistry<SceneDomainTag, LinearLookupStrategy<SCENE_SIZE>, true, SCENE_SIZE>;

    /**
     * @brief Entity manager used for scene handles and scene components.
     */
    using SceneEntityManager = EntityManager<SceneHandle, SceneEntityRegistry, SCENE_SIZE>;


}