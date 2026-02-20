/**
 * @file GameObjectPoolManager.ixx
 * @brief Manager for GameObject pooling and lifecycle management.
 */
module;

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>


export module helios.engine.runtime.pooling.GameObjectPoolManager;

import helios.engine.core.data.GameObjectPoolId;

import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.pooling.GameObjectPool;
import helios.engine.runtime.pooling.GameObjectPoolRegistry;

import helios.engine.runtime.pooling.GameObjectPoolConfig;
import helios.engine.runtime.pooling.components.PrefabIdComponent;

import helios.engine.ecs.EntityHandle;
import helios.core.types;

import helios.engine.runtime.pooling.GameObjectPoolSnapshot;

export namespace helios::engine::runtime::pooling {

    /**
     * @brief High-level manager for GameObject pooling operations.
     *
     * @details GameObjectPoolManager provides a unified interface for managing
     * multiple GameObjectPools. It handles pool configuration, initialization,
     * and the acquire/release lifecycle of pooled GameObjects.
     *
     * The manager integrates with the GameWorld to clone prefabs during pool
     * initialization and to look up GameObjects by their EntityHandle during
     * acquire/release operations.
     *
     * ## Lifecycle
     *
     * 1. **Configuration**: Add pool configurations via `addPoolConfig()`.
     * 2. **Initialization**: Call `init()` to create pools and populate them
     *    with cloned prefab instances. This **locks** the pools.
     * 3. **Runtime**: Use `acquire()` and `release()` to manage entity lifecycle.
     *
     * ## Pool Locking
     *
     * After `init()` completes, each pool is **locked**. A locked pool:
     * - Cannot accept new EntityHandles via `addInactive()`
     * - Has its sparse arrays sized based on min/max EntityIds
     * - Is ready for O(1) acquire/release operations
     *
     * This design optimizes memory layout but means the pool size is fixed
     * at initialization time.
     *
     * ## Trade-offs
     *
     * - **Fixed Capacity**: Pool size cannot grow after initialization. If more
     *   entities are needed, `acquire()` returns nullptr when exhausted.
     * - **Memory Overhead**: Sparse arrays are sized for the EntityId range,
     *   which may waste memory if EntityIds are not contiguous.
     * - **Initialization Cost**: All prefab clones are created upfront during
     *   `init()`, which may cause a startup delay for large pools.
     *
     * ## Example
     *
     * ```cpp
     * GameObjectPoolManager poolManager;
     *
     * // Create prefab
     * auto bulletPrefab = gameWorld.addGameObject();
     * bulletPrefab.add<RenderableComponent>(mesh, material);
     * bulletPrefab.add<Move2DComponent>();
     * bulletPrefab.setActive(false);
     *
     * auto config = std::make_unique<GameObjectPoolConfig>(
     *     bulletPoolId, bulletPrefab, 100
     * );
     * poolManager.addPoolConfig(std::move(config));
     *
     * poolManager.init(gameWorld);
     *
     * // Acquire returns std::optional<GameObject>
     * if (auto bullet = poolManager.acquire(bulletPoolId)) {
     *     bullet->get<TranslationStateComponent>()->setTranslation(spawnPos);
     *     bullet->setActive(true);
     * }
     *
     * // Release back to pool
     * poolManager.release(bulletPoolId, bullet->entityHandle());
     * ```
     *
     * @see GameObjectPool
     * @see GameObjectPoolRegistry
     * @see Manager
     */
    class GameObjectPoolManager : public helios::engine::runtime::world::Manager {

        /**
         * @brief Registry of GameObjectPools for entity recycling.
         *
         * @details Pools enable efficient reuse of GameObjects without repeated
         * allocation/deallocation. Each pool is identified by a GameObjectPoolId.
         */
        helios::engine::runtime::pooling::GameObjectPoolRegistry pools_{};

        /**
         * @brief Non-owning pointer to the associated GameWorld.
         *
         * @details Set during `init()`. Used for cloning prefabs and looking up
         * GameObjects by their EntityHandle.
         */
        helios::engine::runtime::world::GameWorld* gameWorld_ = nullptr;

        /**
         * @brief Pending pool configurations awaiting initialization.
         *
         * @details Configurations are consumed during `init()` to create and
         * populate the actual pools.
         */
        std::unordered_map<
            helios::engine::core::data::GameObjectPoolId,
            std::unique_ptr<GameObjectPoolConfig>> poolConfigs_;

        /**
         * @brief Populates a pool with cloned prefab instances and locks it.
         *
         * @details Clones the prefab GameObject until the pool reaches its
         * configured capacity. Each clone is:
         * 1. Added to the GameWorld
         * 2. Immediately deactivated (`setActive(false)`)
         * 3. Prepared for pooling (`onRelease()`)
         * 4. Registered as inactive in the pool
         *
         * After all clones are created, the pool is **locked** via `lock()`.
         * Locking finalizes the sparse array sizing based on the min/max EntityIds
         * and enables O(1) acquire/release operations. Once locked, no new
         * EntityHandles can be added to the pool.
         *
         * @param gameObjectPoolId The pool to fill.
         * @param gameObjectPrefab The prefab to clone.
         *
         * @post The pool is locked and ready for acquire/release operations.
         */
        void fillPool(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            helios::engine::ecs::GameObject gameObjectPrefab
        ) {
            helios::engine::ecs::EntityHandle entityHandle{};

            auto* gameObjectPool = pool(gameObjectPoolId);
            
            const size_t used  = gameObjectPool->activeCount() + gameObjectPool->inactiveCount();
            const size_t space = used < gameObjectPool->size() ? gameObjectPool->size() - used : 0;

            for (size_t i = 0; i < space; i++) {
                helios::engine::ecs::GameObject go = gameWorld_->clone(gameObjectPrefab);
                go.setActive(false);
                go.onRelease();
                gameObjectPool->addInactive(go.entityHandle());
            }

            gameObjectPool->lock();
        }
        
    public:

        /**
         * @brief Registers a pool configuration for later initialization.
         *
         * @details The configuration specifies the pool ID, capacity, and prefab
         * to use for cloning. Configurations are processed during `init()`.
         *
         * @param gameObjectPoolConfig The pool configuration to register.
         *
         * @return Reference to this manager for method chaining.
         *
         * @throws std::runtime_error If a pool with the same ID is already registered.
         */
        GameObjectPoolManager& addPoolConfig(std::unique_ptr<GameObjectPoolConfig> gameObjectPoolConfig) {

            if (poolConfigs_.contains(gameObjectPoolConfig->gameObjectPoolId)) {
                throw std::runtime_error("Pool already registered with this manager");
            }

            poolConfigs_[gameObjectPoolConfig->gameObjectPoolId] = std::move(gameObjectPoolConfig);

            return *this;
        }

        /**
         * @brief Returns a snapshot of the pool's current state.
         *
         * @details Provides the active and inactive counts for monitoring and
         * debugging purposes.
         *
         * @param gameObjectPoolId The pool to query.
         *
         * @return A GameObjectPoolSnapshot containing active and inactive counts.
         */
        [[nodiscard]] helios::engine::runtime::pooling::GameObjectPoolSnapshot poolSnapshot(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId
        ) const {
            auto* gameObjectPool = pool(gameObjectPoolId);

            return {
                gameObjectPool->activeCount(), gameObjectPool->inactiveCount()
            };
        }
        
        /**
         * @brief Releases a GameObject back to its pool.
         *
         * @details Marks the entity as inactive in both the pool and the
         * GameWorld. Calls `onRelease()` on the GameObject to trigger
         * component cleanup hooks, then deactivates it.
         *
         * @param gameObjectPoolId The pool that owns this entity.
         * @param entityHandle The EntityHandle of the entity to release.
         *
         * @return Optional containing the released GameObject if found,
         *         std::nullopt if the entity was not found in the GameWorld.
         */
        std::optional<helios::engine::ecs::GameObject> release(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const helios::engine::ecs::EntityHandle& entityHandle
        ) {
            auto* gameObjectPool = pool(gameObjectPoolId);
            
            auto worldGo = gameWorld_->find(entityHandle);

            if (worldGo) {
                if (gameObjectPool->release(entityHandle)) {
                    worldGo->onRelease();
                    worldGo->setActive(false);
                }
            }

            return worldGo;
        }

        /**
         * @brief Acquires an inactive GameObject from the pool.
         *
         * @details Retrieves the next available inactive entity and calls
         * `onAcquire()` to trigger component initialization hooks. The caller
         * is responsible for activating the entity via `setActive(true)`.
         *
         * If an acquired entity is no longer valid in the GameWorld (stale handle),
         * it is removed from the pool and the next available entity is tried.
         *
         * @param gameObjectPoolId The pool to acquire from.
         *
         * @return Optional containing the acquired GameObject if available,
         *         std::nullopt if the pool is exhausted.
         */
        [[nodiscard]] std::optional<helios::engine::ecs::GameObject> acquire(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId
        )  {
            helios::engine::ecs::EntityHandle entityHandle{};

            auto* gameObjectPool = pool(gameObjectPoolId);

            while (gameObjectPool->acquire(entityHandle)) {

                auto worldGo = gameWorld_->find(entityHandle);

                if (worldGo) {
                    worldGo->onAcquire();
                    return worldGo;
                }

                // we assume the pool is owned by this gameWorld,
                // so removing this entityHandle does not impact another gameWorld that is
                // using this pool
                gameObjectPool->releaseAndRemove(entityHandle);
            }

            return std::nullopt;

        }

        /**
         * @brief Initializes all registered pools.
         *
         * @details Creates GameObjectPool instances from the pending configurations
         * and populates them by cloning the specified prefabs into the GameWorld.
         *
         * @param gameWorld The GameWorld to associate with this manager.
         */
        void init(helios::engine::runtime::world::GameWorld& gameWorld) override {

            gameWorld_ = &gameWorld;
            
            for (const auto& [gameObjectPoolId, poolConfig] : poolConfigs_) {

                auto pool = std::make_unique<helios::engine::runtime::pooling::GameObjectPool>(
                    poolConfig->amount);

                pools_.addPool(gameObjectPoolId, std::move(pool));

                for (auto [entity, pic] : gameWorld.view<helios::engine::runtime::pooling::components::PrefabIdComponent>().whereEnabled()) {
                    if (pic->prefabId() == poolConfig->prefabId) {
                        fillPool(gameObjectPoolId, entity);
                        break;
                    }
                }

            }

        };

        /**
         * @brief Manager flush callback (currently unused).
         *
         * @details Reserved for future use. May be used to process deferred
         * release requests or pool maintenance operations.
         *
         * @param gameWorld The associated GameWorld.
         * @param update_context The current frame's update context.
         */
        void flush(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::world::UpdateContext& update_context
        ) noexcept override {

        }

        /**
         * @brief Retrieves a pool by its ID.
         *
         * @param gameObjectPoolId The pool ID to look up.
         *
         * @return Pointer to the GameObjectPool.
         *
         * @pre The pool must be registered with this manager.
         */
        [[nodiscard]] GameObjectPool* pool(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId
        ) const {
            assert(pools_.has(gameObjectPoolId) && "GameObjectPoolId not registered with this manager");
            return pools_.pool(gameObjectPoolId);
        }

        /**
         * @brief Resets all pools by releasing all active GameObjects.
         *
         * @details Iterates through all registered pools and releases every active
         * entity back to its pool. This effectively returns all pooled objects to
         * their inactive state without destroying them.
         *
         * Useful for level transitions or game restarts where all pooled objects
         * should be recycled.
         */
        void reset() override {

            for (auto& [poolId, poolPtr]  : pools_.pools()) {

                auto activeSpan = poolPtr->activeGameObjects();
                auto toRelease = std::vector(activeSpan.begin(), activeSpan.end());

                for (auto entityHandle : toRelease) {
                    release(poolId, entityHandle);
                }


            }

        }

    };

}

