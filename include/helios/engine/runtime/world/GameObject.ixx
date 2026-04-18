/**
 * @file GameObject.ixx
 * @brief Lightweight facade alias for runtime game-object entities.
 */
module;

#include <cassert>
#include <type_traits>
#include <typeindex>
#include <generator>


export module helios.engine.runtime.world.GameObject;

import helios.engine.runtime.world.GameObjectEntityManager;
import helios.ecs.Entity;

using namespace helios::ecs;
export namespace helios::engine::runtime::world {

    /**
     * @brief Alias for an entity facade backed by `GameObjectEntityManager`.
     */
    using GameObject = Entity<GameObjectEntityManager>;

} // namespace helios::engine::modules
