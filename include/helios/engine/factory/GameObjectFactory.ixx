/**
 * @file GameObjectFactory.ixx
 * @brief Factory for creating and pooling GameObject instances from a prefab template.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.factory.GameObjectFactory;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.GameWorld;

import helios.engine.core.data.GameObjectPool;
import helios.engine.core.data.GameObjectPoolId;

import helios.engine.game.pool.components.PoolIdComponent;

export namespace helios::engine::factory {

    /**
     * @brief Factory class for mass-producing GameObjects from a prefab.
     *
     * @details GameObjectFactory holds a prototype (prefab) of a GameObject and provides
     * methods to fill a GameObjectPool with clones of that prefab. This enables efficient
     * spawning by pre-allocating objects and reusing them via pooling.
     *
     * The factory owns the prefab exclusively and clones it into the GameWorld as needed.
     * Created objects are immediately marked inactive and registered with the pool.
     *
     * Example usage:
     * ```cpp
     * auto prefab = std::make_unique<GameObject>();
     * prefab->add<RenderableComponent>(bulletMesh);
     * prefab->add<Move2DComponent>();
     *
     * GameObjectFactory factory(std::move(prefab));
     * factory.fillPool(gameWorld, bulletPool);
     * ```
     */
    class GameObjectFactory {

    private:

        /**
         * @brief The prototype GameObject used as a template for cloning.
         *
         * This prefab is cloned into the GameWorld when filling a pool.
         * The factory owns this prefab exclusively.
         */
        std::unique_ptr<helios::engine::ecs::GameObject> gameObjectPrefab_ = nullptr;


    public:

        /**
         * @brief Constructs a GameObjectFactory with a prefab template.
         *
         * @param gameObjectPrefab The prototype GameObject to clone. Must not be nullptr.
         *
         * @pre `gameObjectPrefab != nullptr`
         */
        explicit GameObjectFactory(
            std::unique_ptr<helios::engine::ecs::GameObject> gameObjectPrefab
        ) : gameObjectPrefab_(std::move(gameObjectPrefab)) {
            assert(gameObjectPrefab_ != nullptr && "unexpected nullptr for prefab");
            assert(gameObjectPrefab_->get<helios::engine::game::pool::components::PoolIdComponent>()
                && "Prefab should provide PoolIdComponent");

        }

        /**
         * @brief Fills a GameObjectPool with cloned instances of the prefab.
         *
         * @details Calculates the remaining capacity in the pool and creates that many
         * clones of the prefab. Each clone is:
         * - Added to the GameWorld via `clone()`
         * - Marked as inactive (`setActive(false)`)
         * - Registered with the pool's inactive list
         *
         * This method is typically called once during initialization to pre-populate
         * the pool with ready-to-use objects.
         *
         * @param gameWorld The GameWorld that will own the cloned GameObjects.
         * @param gameObjectPool The pool to fill with inactive clones.
         */
        void fillPool(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::core::data::GameObjectPool& gameObjectPool
        ) {
            const size_t used  = gameObjectPool.activeCount() + gameObjectPool.inactiveCount();
            const size_t space = used < gameObjectPool.size() ? gameObjectPool.size() - used : 0;

            for (size_t i = 0; i < space; i++) {
                helios::engine::ecs::GameObject* go = gameWorld.clone(*gameObjectPrefab_);
                if (go) {
                    go->setActive(false);
                    gameObjectPool.addInactive(go->guid());
                }
            }

        }


    };



}