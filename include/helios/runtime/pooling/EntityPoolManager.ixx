/**
 * @file EntityPoolManager.ixx
 * @brief Manager for Entity pooling and lifecycle management.
 */
module;

#include <cassert>
#include <memory>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <cstddef>


export module helios.runtime.pooling.EntityPoolManager;

import helios.ecs.Entity;
import helios.runtime.pooling.types.EntityPoolId;

import helios.runtime.world.UpdateContext;

import helios.runtime.world.EngineWorld;
import helios.runtime.pooling.EntityPool;
import helios.runtime.pooling.EntityPoolRegistry;

import helios.runtime.pooling.EntityPoolConfig;
import helios.runtime.pooling.components.PrefabIdComponent;

import helios.runtime.messaging.command.CommandHandlerRegistry;


import helios.ecs.types.EntityHandle;
import helios.core.types;
import helios.runtime.world.tags;

import helios.runtime.pooling.EntityPoolSnapshot;

using namespace helios::runtime::messaging::command;
export namespace helios::runtime::pooling {

    /**
     * @brief High-level manager for Entity pooling operations.
     *
     * @details EntityPoolManager provides a unified interface for managing
     * multiple EntityPools. It handles pool configuration, initialization,
     * and the acquire/release lifecycle of pooled Entities.
     *
     * The manager integrates with the GameWorld to clone prefabs during pool
     * initialization and to look up Entities by their EntityHandle during
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
     * EntityPoolManager poolManager;
     *
     * // Create prefab
     * auto bulletPrefab = gameWorld.addEntity();
     * bulletPrefab.add<RenderableComponent>(mesh, material);
     * bulletPrefab.add<Move2DComponent>();
     * bulletPrefab.setActive(false);
     *
     * auto config = std::make_unique<EntityPoolConfig>(
     *     bulletPoolId, bulletPrefab, 100
     * );
     * poolManager.addPoolConfig(std::move(config));
     *
     * poolManager.init(gameWorld);
     *
     * // Acquire returns std::optional<Entity>
     * if (auto bullet = poolManager.acquire(bulletPoolId)) {
     *     bullet->get<TranslationStateComponent>()->setTranslation(spawnPos);
     *     bullet->setActive(true);
     * }
     *
     * // Release back to pool
     * poolManager.release(bulletPoolId, bullet->entityHandle());
     * ```
     *
     * @see EntityPool
     * @see EntityPoolRegistry
     * @see Manager
     */
    template<typename TEntity>
    class EntityPoolManager {

        using Handle_type = typename TEntity::Handle_type;
        using Entity_type = TEntity;
        
        /**
         * @brief Registry of EntityPools for entity recycling.
         *
         * @details Pools enable efficient reuse of Entities without repeated
         * allocation/deallocation. Each pool is identified by a EntityPoolId.
         */
        helios::runtime::pooling::EntityPoolRegistry<Handle_type> pools_{};

        /**
         * @brief Non-owning pointer to the associated GameWorld.
         *
         * @details Set during `init()`. Used for cloning prefabs and looking up
         * Entities by their EntityHandle.
         */
        helios::runtime::world::EngineWorld* engineWorld_ = nullptr;

        /**
         * @brief Pending pool configurations awaiting initialization.
         *
         * @details Configurations are consumed during `init()` to create and
         * populate the actual pools.
         */
        std::unordered_map<
            helios::runtime::pooling::types::EntityPoolId,
            std::unique_ptr<EntityPoolConfig>> poolConfigs_;

        /**
         * @brief Populates a pool with cloned prefab instances and locks it.
         *
         * @details Clones the prefab Entity until the pool reaches its
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
         * @param entityPoolId The pool to fill.
         * @param entityPrefab The prefab to clone.
         *
         * @post The pool is locked and ready for acquire/release operations.
         */
        void fillPool(
            const helios::runtime::pooling::types::EntityPoolId entityPoolId,
            TEntity entityPrefab
        ) {
            Handle_type entityHandle{};

            auto* entityPool = pool(entityPoolId);
            
            const size_t used  = entityPool->activeCount() + entityPool->inactiveCount();
            const size_t space = used < entityPool->size() ? entityPool->size() - used : 0;

            for (size_t i = 0; i < space; i++) {
                Entity_type go = engineWorld_->clone(entityPrefab.handle());
                go.setActive(false);
                go.onRelease();
                entityPool->addInactive(go.handle());
            }

            entityPool->lock();
        }
        
    public:
        using EngineRoleTag = helios::runtime::tags::ManagerRole;


        explicit EntityPoolManager(helios::runtime::world::EngineWorld& engineWorld) : engineWorld_(&engineWorld) {}

        /**
         * @brief Registers a pool configuration for later initialization.
         *
         * @details The configuration specifies the pool ID, capacity, and prefab
         * to use for cloning. Configurations are processed during `init()`.
         *
         * @param entityPoolConfig The pool configuration to register.
         *
         * @return Reference to this manager for method chaining.
         *
         * @throws std::runtime_error If a pool with the same ID is already registered.
         */
        EntityPoolManager& addPoolConfig(std::unique_ptr<EntityPoolConfig> entityPoolConfig) {

            if (poolConfigs_.contains(entityPoolConfig->entityPoolId)) {
                throw std::runtime_error("Pool already registered with this manager");
            }

            poolConfigs_[entityPoolConfig->entityPoolId] = std::move(entityPoolConfig);

            return *this;
        }

        /**
         * @brief Returns a snapshot of the pool's current state.
         *
         * @details Provides the active and inactive counts for monitoring and
         * debugging purposes.
         *
         * @param entityPoolId The pool to query.
         *
         * @return A EntityPoolSnapshot containing active and inactive counts.
         */
        [[nodiscard]] helios::runtime::pooling::EntityPoolSnapshot poolSnapshot(
            const helios::runtime::pooling::types::EntityPoolId entityPoolId
        ) const {
            auto* entityPool = pool(entityPoolId);

            return {
                entityPool->activeCount(), entityPool->inactiveCount()
            };
        }
        
        /**
         * @brief Releases a Entity back to its pool.
         *
         * @details Marks the entity as inactive in both the pool and the
         * GameWorld. Calls `onRelease()` on the Entity to trigger
         * component cleanup hooks, then deactivates it.
         *
         * @param entityPoolId The pool that owns this entity.
         * @param entityHandle The EntityHandle of the entity to release.
         *
         * @return Optional containing the released Entity if found,
         *         std::nullopt if the entity was not found in the GameWorld.
         */
        std::optional<TEntity> release(
            const helios::runtime::pooling::types::EntityPoolId entityPoolId,
            const Handle_type& entityHandle
        ) {
            auto* entityPool = pool(entityPoolId);
            
            auto worldGo = engineWorld_->find<Handle_type>(entityHandle);

            if (worldGo) {
                if (entityPool->release(entityHandle)) {
                    worldGo->onRelease();
                    worldGo->setActive(false);
                }
            }

            return worldGo;
        }

        /**
         * @brief Acquires an inactive Entity from the pool.
         *
         * @details Retrieves the next available inactive entity and calls
         * `onAcquire()` to trigger component initialization hooks. The caller
         * is responsible for activating the entity via `setActive(true)`.
         *
         * If an acquired entity is no longer valid in the GameWorld (stale handle),
         * it is removed from the pool and the next available entity is tried.
         *
         * @param entityPoolId The pool to acquire from.
         *
         * @return Optional containing the acquired Entity if available,
         *         std::nullopt if the pool is exhausted.
         */
        [[nodiscard]] std::optional<TEntity> acquire(
            const helios::runtime::pooling::types::EntityPoolId entityPoolId
        )  {
            Handle_type entityHandle{};

            auto* entityPool = pool(entityPoolId);

            while (entityPool->acquire(entityHandle)) {

                auto worldGo = engineWorld_->find(entityHandle);

                if (worldGo) {
                    worldGo->onAcquire();
                    return worldGo;
                }

                // we assume the pool is owned by this gameWorld,
                // so removing this entityHandle does not impact another gameWorld that is
                // using this pool
                entityPool->releaseAndRemove(entityHandle);
            }

            return std::nullopt;

        }

        /**
         * @brief Initializes all registered pools.
         *
         * @details Creates EntityPool instances from the pending configurations
         * and populates them by cloning the specified prefabs into the GameWorld.
         *
         * @param gameWorld The GameWorld to associate with this manager.
         */
        void init(CommandHandlerRegistry& commandHandlerRegistry) {

            
            for (const auto& [entityPoolId, poolConfig] : poolConfigs_) {

                auto pool = std::make_unique<helios::runtime::pooling::EntityPool<Handle_type>>(
                    poolConfig->amount);

                pools_.addPool(entityPoolId, std::move(pool));

                for (auto [entity, pic] : engineWorld_->view<
                    Handle_type,
                    helios::runtime::pooling::components::PrefabIdComponent<Handle_type>>().whereEnabled()) {
                    if (pic->prefabId() == poolConfig->prefabId) {
                        fillPool(entityPoolId, entity);
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
            helios::runtime::world::UpdateContext& update_context
        ) noexcept {

        }

        /**
         * @brief Retrieves a pool by its ID.
         *
         * @param entityPoolId The pool ID to look up.
         *
         * @return Pointer to the EntityPool.
         *
         * @pre The pool must be registered with this manager.
         */
        [[nodiscard]] EntityPool<Handle_type>* pool(
            const helios::runtime::pooling::types::EntityPoolId entityPoolId
        ) const {
            assert(pools_.has(entityPoolId) && "EntityPoolId not registered with this manager");
            return pools_.pool(entityPoolId);
        }

        /**
         * @brief Resets all pools by releasing all active Entities.
         *
         * @details Iterates through all registered pools and releases every active
         * entity back to its pool. This effectively returns all pooled objects to
         * their inactive state without destroying them.
         *
         * Useful for level transitions or game restarts where all pooled objects
         * should be recycled.
         *
         * @see reset(const types::EntityPoolId)
         */
        void reset() {
            for (auto& [poolId, _]  : pools_.pools()) {
                reset(poolId);
            }
        }

        /**
         * @brief Reset the pool with the specified poolId.
         *
         * @details Releases every active entity back to its pool specified by poolId.
         * This effectively returns all pooled objects to their inactive state without
         * destroying them.
         *
         * @param poolId The ID of the pool to reset.
         */
        void reset(const types::EntityPoolId poolId) {

            const auto poolPtr = pool(poolId);

            auto activeSpan = poolPtr->activeEntities();
            auto toRelease = std::vector(activeSpan.begin(), activeSpan.end());
            for (auto entityHandle : toRelease) {
                release(poolId, entityHandle);
            }
        }
    };

}

