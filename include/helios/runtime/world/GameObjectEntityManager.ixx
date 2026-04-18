/**
 * @file GameObjectEntityManager.ixx
 * @brief Runtime ECS registry and manager aliases for game-object entities.
 */
module;

#include "helios/helios_config.h"


export module helios.runtime.world.GameObjectEntityManager;

import helios.ecs;
import helios.runtime.world.types;

constexpr size_t GAMEOBJECT_SIZE = DEFAULT_GAMEOBJECT_CAPACITY;


using namespace helios::ecs;
using namespace helios::ecs::strategies;
using namespace helios::runtime::world::types;
export namespace helios::runtime::world {

    /**
     * @brief Entity registry configuration used for game objects.
     */
    using GameObjectEntityRegistry = EntityRegistry<GameObjectId, HashedLookupStrategy, false, GAMEOBJECT_SIZE>;

    /**
     * @brief Entity manager alias for game-object handles.
     */
    using GameObjectEntityManager = EntityManager<GameObjectHandle, GameObjectEntityRegistry, GAMEOBJECT_SIZE>;

}