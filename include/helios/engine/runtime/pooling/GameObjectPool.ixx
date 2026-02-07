/**
 * @file GameObjectPool.ixx
 * @brief Object pool for efficient GameObject management and recycling.
 *
 * Provides a memory-efficient pooling mechanism for frequently spawned and despawned
 * GameObjects, such as projectiles, particles, or enemies. Pre-allocates objects at
 * construction time and reuses them to avoid runtime allocation overhead.
 */
module;

#include <memory>
#include <unordered_map>
#include <vector>
#include <cassert>
#include <limits>


export module helios.engine.runtime.pooling.GameObjectPool;

import helios.util.Guid;
import helios.engine.ecs.EntityHandle;
import helios.engine.core.data;

export namespace helios::engine::runtime::pooling {


    /**
     * @brief Object pool for efficient GameObject lifecycle management.
     *
     * @details GameObjectPool manages a fixed-size collection of GameObject identifiers,
     * tracking which objects are currently active (in-use) and which are inactive (available).
     * This pattern eliminates runtime allocation overhead for frequently spawned entities
     * like projectiles, particles, or enemies.
     *
     * The pool uses O(1) operations for both acquire and release:
     * - **acquire**: Pops from the inactive list and adds to active tracking
     * - **release**: Swap-and-pop removal from active list, push to inactive
     *
     * GameObjects themselves are owned by GameWorld; this pool only tracks their EntityHandles.
     *
     * @todo Prevent duplicate EntityHandles from being added to the pool.
     */
    class GameObjectPool {


    protected:

        /**
         * @brief Maps active GameObject EntityIds to their index in activeGameObjects_.
         *
         * Enables O(1) lookup for release operations.
         */
        std::vector<size_t> activeIndex_;

        /**
         * @brief Tracks version numbers for active EntityHandles.
         *
         * Used to validate that a release operation targets the correct entity version.
         */
        std::vector<size_t> versionIndex_;

        /**
         * @brief List of EntityHandles for currently active (in-use) GameObjects.
         */
        std::vector<helios::engine::ecs::EntityHandle> activeGameObjects_;

        /**
         * @brief List of EntityHandles for currently inactive (available) GameObjects.
         */
        std::vector<helios::engine::ecs::EntityHandle> inactiveGameObjects_;

        /**
         * @brief The maximum number of objects this pool manages.
         */
        size_t poolSize_ = 0;

        /**
         * @brief Minimum EntityId in the pool (used for sparse array offset).
         */
        helios::engine::core::data::EntityId minEntityId_ = std::numeric_limits<helios::engine::core::data::EntityId>::max();

        /**
         * @brief Maximum EntityId in the pool (used for sparse array sizing).
         */
        helios::engine::core::data::EntityId maxEntityId_ = std::numeric_limits<helios::engine::core::data::EntityId>::lowest();

        /**
         * @brief Offset for sparse array indexing (equals minEntityId_ after lock).
         */
        size_t delta_ = 0;

        /**
         * @brief Unique identifier for this pool instance.
         */
        const helios::util::Guid guid_;

        /**
         * @brief True if the pool is locked and ready for acquire/release operations.
         */
        bool locked_ = false;

    public:


        /**
         * @brief Constructs a GameObjectPool with the specified capacity.
         *
         * @details Pre-allocates internal storage for the given pool size.
         * The pool starts empty; use `addInactive()` or a factory to populate it.
         *
         * @param poolSize The maximum number of GameObjects this pool can manage.
         */
        explicit GameObjectPool(
            size_t poolSize
        ) :
        poolSize_(poolSize),
        guid_(helios::util::Guid::generate()) {
            activeGameObjects_.reserve(poolSize);
            inactiveGameObjects_.reserve(poolSize);
        }


        /**
         * @brief Returns the unique identifier of this pool.
         *
         * @return The Guid assigned to this pool instance.
         */
        [[nodiscard]] helios::util::Guid guid() const noexcept {
            return guid_;
        }

        /**
         * @brief Returns the maximum capacity of this pool.
         *
         * @return The pool size specified at construction.
         */
        [[nodiscard]] size_t size() const noexcept {
            return poolSize_;
        }

        /**
         * @brief Acquires an inactive GameObject from the pool.
         *
         * @details Removes a EntityHandle from the inactive list and adds it to the active
         * tracking structures. The caller is responsible for activating the actual
         * GameObject in the GameWorld.
         *
         * @param[out] entityHandle Receives the EntityHandle of the acquired object on success.
         *
         * @return True if an object was acquired, false if the pool is exhausted.
         */
        [[nodiscard]] bool acquire(helios::engine::ecs::EntityHandle& entityHandle) {

            if (inactiveGameObjects_.empty()) {
                return false;
            }

            entityHandle = inactiveGameObjects_.back();
            inactiveGameObjects_.pop_back();


            auto idx = entityHandle.entityId - delta_;

            if (activeIndex_.size() <= idx) {
                activeIndex_.resize(idx + 1, helios::engine::core::data::EntityTombstone);
                versionIndex_.resize(idx + 1, helios::engine::core::data::EntityTombstone);
            }

            activeIndex_[idx] = activeGameObjects_.size();
            versionIndex_[idx] = entityHandle.versionId;

            activeGameObjects_.push_back(entityHandle);

            return true;
        }

        /**
         * @brief Checks if the pool is locked.
         *
         * @return True if the pool is locked and ready for acquire/release operations.
         */
        [[nodiscard]] bool isLocked() const noexcept {
            return locked_;
        }

        /**
         * @brief Locks the pool for acquire/release operations.
         *
         * @details After locking, no more EntityHandles can be added via `addInactive()`.
         * The sparse arrays are sized based on the min/max EntityIds added.
         */
        void lock() noexcept {
            locked_ = true;
            delta_ = minEntityId_;
            activeIndex_.resize(maxEntityId_ - delta_ + 1, helios::engine::core::data::EntityTombstone);
            versionIndex_.resize(maxEntityId_ - delta_ + 1, helios::engine::core::data::EntityTombstone);
        }

        /**
         * @brief Adds a EntityHandle to the inactive list without acquiring it.
         *
         * @details Used during pool initialization to register pre-created GameObjects.
         * Fails if the pool is already at capacity.
         *
         * @param entityHandle The EntityHandle of the GameObject to add.
         *
         * @return True if added successfully, false if pool is full.
         */
        bool addInactive(const helios::engine::ecs::EntityHandle entityHandle) {

            assert(!locked_ && "Pool is locked");

            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");

            const size_t used = (activeCount() + inactiveCount());

            minEntityId_ = std::min(minEntityId_, entityHandle.entityId);
            maxEntityId_ = std::max(maxEntityId_, entityHandle.entityId);


            if (used < size()) {
                inactiveGameObjects_.push_back(entityHandle);
                return true;
            }

            return false;
        }

        /**
         * @brief Releases a GameObject back to the pool by its EntityHandle.
         *
         * @details
         * Validates the EntityHandle against both the GameWorld and the active tracking list.
         * Uses swap-and-pop for O(1) removal from the active list. The object is
         * marked inactive and added to the inactive list for future acquisition.
         *
         * @param entityHandle The unique identifier of the GameObject to release.
         *
         * @return True if the object was successfully released, false if the EntityHandle
         *         was not found in the GameWorld or not tracked as active.
         */
        bool release(const helios::engine::ecs::EntityHandle entityHandle) {

            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");

            assert(entityHandle.entityId >= delta_  && "Unexpected entityHandle");

            const auto sparseIdx = entityHandle.entityId - delta_;

            assert(sparseIdx < activeIndex_.size() && "Unexpected sparse index");

            const auto denseIndex = activeIndex_[sparseIdx];
            if (denseIndex == helios::engine::core::data::EntityTombstone) {
                return false;
            }

            assert(versionIndex_[sparseIdx] == entityHandle.versionId && "Version mismatch");

            auto lastEntityHandle = activeGameObjects_.back();

            if (denseIndex != activeGameObjects_.size() - 1) {
                // swap the last entityHandle in activeGameObjects with the
                // entityHandle to remove, effectively overwriting entityHandle
                // to release with a currently active entityHandle
                activeGameObjects_[denseIndex] = lastEntityHandle;
                activeIndex_[lastEntityHandle.entityId-delta_] = denseIndex;
                versionIndex_[lastEntityHandle.entityId-delta_] = lastEntityHandle.versionId;
            }


            // the swap operation has create a duplicate entry,
            // remove the one at the tail
            activeGameObjects_.pop_back();

            // clear the queried entityHandle from active index and update
            // inactiveGameObjects
            activeIndex_[sparseIdx] = helios::engine::core::data::EntityTombstone;
            versionIndex_[sparseIdx] = helios::engine::core::data::EntityTombstone;

            inactiveGameObjects_.push_back(entityHandle);

            return true;
        }

        /**
         * @brief Releases and permanently removes a GameObject from the pool.
         *
         * @details Unlike `release()`, this method does not add the EntityHandle back to the
         * inactive list. Use this when a pooled object is being destroyed rather than
         * recycled.
         *
         * @param entityHandle The unique identifier of the GameObject to remove.
         *
         * @return True if removed successfully, false if EntityHandle was not active.
         */
        bool releaseAndRemove(const helios::engine::ecs::EntityHandle entityHandle) {

            assert(entityHandle.isValid() && "Unexpected invalid entityHandle");

            const auto sparseIdx = entityHandle.entityId - delta_;

            assert(sparseIdx < activeIndex_.size() && "Unexpected sparse index");

            const auto denseIndex = activeIndex_[sparseIdx];

            if (denseIndex == helios::engine::core::data::EntityTombstone) {
                return false;
            }

            assert(versionIndex_[sparseIdx] == entityHandle.versionId && "Version mismatch");

            if (denseIndex != activeGameObjects_.size() - 1) {
                const auto lastEntityHandle = activeGameObjects_.back();
                activeIndex_[lastEntityHandle.entityId - delta_] = denseIndex;
                versionIndex_[lastEntityHandle.entityId - delta_] = lastEntityHandle.versionId;
                activeGameObjects_[denseIndex] = lastEntityHandle;
            }

            activeGameObjects_.pop_back();

            activeIndex_[sparseIdx] = helios::engine::core::data::EntityTombstone;
            versionIndex_[sparseIdx] = helios::engine::core::data::EntityTombstone;

            return true;

        }


        /**
         * @brief Returns the number of active game objects.
         *
         * @return The number of active game objects.
         */
        [[nodiscard]] size_t activeCount() const noexcept {
            return activeGameObjects_.size();
        }

        /**
         * @brief Returns the number of inactive game objects.
         *
         * @return The number of inactive game objects.
         */
        [[nodiscard]] size_t inactiveCount() const noexcept {
            return inactiveGameObjects_.size();
        }
    };

}
