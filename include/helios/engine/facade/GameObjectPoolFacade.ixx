/**
 * @file GameObjectPoolFacade.ixx
 * @brief Facade for coordinating GameObjectPool and GameWorld interactions.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.facade.GameObjectPoolFacade;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.pooling.GameObjectPool;

import helios.util.Guid;

export namespace helios::engine::facade {

    /**
     * @brief Facade class that coordinates object pooling with the game world.
     *
     * @details `GameObjectPoolFacade` provides a convenient interface for acquiring
     * and releasing pooled GameObjects while maintaining synchronization between
     * the GameObjectPool and the GameWorld.
     *
     * This facade ensures that released objects are deactivated in the GameWorld and
     * acquired objects are validated against the GameWorld before use.
     *
     * Stale pool entries (objects no longer in the world) are automatically cleaned up,
     * This process is currently implemented in the acquire()-method.
     */
    class GameObjectPoolFacade {

    public:

        /**
         * @brief Releases a GameObject back to the pool and deactivates it.
         *
         * @details Locates the GameObject in the GameWorld by its entity ID,
         * releases it back to the pool, and sets its active state to `false`.
         * If the entity is not found in the world or the release fails, the
         * object is not modified.
         *
         * @param gameWorld The GameWorld containing the entity.
         * @param gameObjectPool The pool managing the entity lifecycle.
         * @param entityId The unique identifier of the entity to release.
         *
         * @return Pointer to the released GameObject, or `nullptr` if not found.
         */
        helios::engine::ecs::GameObject* release(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::pooling::GameObjectPool& gameObjectPool,
            const helios::util::Guid& entityId
        ) {
            helios::engine::ecs::GameObject* worldGo = gameWorld.find(entityId);

            if (worldGo) {
                if (gameObjectPool.release(entityId)) {
                    worldGo->setActive(false);
                }
            }

            return worldGo;
        }

        /**
         * @brief Acquires an available GameObject from the pool.
         *
         * @details Attempts to acquire an available entity from the pool and
         * validates that it exists in the GameWorld. If an acquired entity is
         * considered stale (no longer present in the world), it is automatically
         * removed from the pool and the next available entity is tried.
         *
         * If a GameObject was successfully identified, its onAcquire()-method
         * is called before its returned.
         *
         * @param gameWorld The GameWorld to validate entities against.
         * @param gameObjectPool The pool to acquire from.
         *
         * @return Pointer to the acquired GameObject, or `nullptr` if the pool
         *         is exhausted or no valid entities are available.
         */
        [[nodiscard]] helios::engine::ecs::GameObject* acquire(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::pooling::GameObjectPool& gameObjectPool
        )  {
            helios::util::Guid guid{helios::util::no_init};

            while (gameObjectPool.acquire(guid)) {

                auto* worldGo = gameWorld.find(guid);

                if (worldGo) {
                    worldGo->onAcquire();
                    return worldGo;
                }

                // we assert the pool is owned by this gameWorld,
                // so removing this guid does not impact another gameWorld that is
                // using this pool
                gameObjectPool.releaseAndRemove(guid);
            }

            return nullptr;

        }

    };


}