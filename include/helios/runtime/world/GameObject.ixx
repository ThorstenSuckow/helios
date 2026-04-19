/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

#include <cassert>
#include <type_traits>
#include <typeindex>


export module helios.runtime.world.GameObject;

import helios.runtime.world.GameObjectEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::runtime::world {

    /**
     * @brief Alias for an entity facade backed by `GameObjectEntityManager`.
     */
    using GameObject = Entity<GameObjectEntityManager>;

} // namespace helios
