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


export module helios.engine.core.data.GameObjectPool;

import helios.util.Guid;

export namespace helios::engine::core::data {


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
     * GameObjects themselves are owned by GameWorld; this pool only tracks their Guids.
     *
     * @todo Prevent duplicate Guids from being added to the pool.
     */
    class GameObjectPool {


    protected:

        /**
         * @brief Maps active GameObject Guids to their index in activeGameObjects_.
         *
         * Enables O(1) lookup for release operations.
         */
        std::unordered_map<helios::util::Guid, size_t> activeIndex_;

        /**
         * @brief List of Guids for currently active (in-use) GameObjects.
         */
        std::vector<helios::util::Guid> activeGameObjects_;

        /**
         * @brief List of Guids for currently inactive (available) GameObjects.
         */
        std::vector<helios::util::Guid> inactiveGameObjects_;

        /**
         * @brief The maximum number of objects this pool manages.
         */
        size_t poolSize_ = 0;


        /**
         * @brief Unique identifier for this pool instance.
         */
        const helios::util::Guid guid_;


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
         * @details Removes a Guid from the inactive list and adds it to the active
         * tracking structures. The caller is responsible for activating the actual
         * GameObject in the GameWorld.
         *
         * @param[out] guid Receives the Guid of the acquired object on success.
         *
         * @return True if an object was acquired, false if the pool is exhausted.
         */
        [[nodiscard]] bool acquire(helios::util::Guid& guid) {

            if (inactiveGameObjects_.empty()) {
                return false;
            }

            guid = inactiveGameObjects_.back();

            inactiveGameObjects_.pop_back();

            activeIndex_[guid] = activeGameObjects_.size();
            activeGameObjects_.push_back(guid);

            return true;
        }

        /**
         * @brief Adds a Guid to the inactive list without acquiring it.
         *
         * @details Used during pool initialization to register pre-created GameObjects.
         * Fails if the pool is already at capacity.
         *
         * @param guid The Guid of the GameObject to add.
         *
         * @return True if added successfully, false if pool is full.
         */
        bool addInactive(const helios::util::Guid& guid) {

            const size_t used = (activeCount() + inactiveCount());

            if (used < size()) {
                inactiveGameObjects_.push_back(guid);
                return true;
            }

            return false;
        }

        /**
         * @brief Releases a GameObject back to the pool by its Guid.
         *
         * @details
         * Validates the Guid against both the GameWorld and the active tracking list.
         * Uses swap-and-pop for O(1) removal from the active list. The object is
         * marked inactive and added to the inactive list for future acquisition.
         *
         * @param guid The unique identifier of the GameObject to release.
         *
         * @return True if the object was successfully released, false if the Guid
         *         was not found in the GameWorld or not tracked as active.
         */
        bool release(const helios::util::Guid& guid) {

            auto it = activeIndex_.find(guid);

            if (it == activeIndex_.end()) {
                return false;
           }


            size_t idx = it->second;
            auto lastGuid = activeGameObjects_.back();

            // swap the last guid in activeGameObjects with the
            // guid to remove, effectively overwriting guid
            // to release with a currently active guid
            activeGameObjects_[idx] = lastGuid;
            activeIndex_[lastGuid] = idx;

            // the swap operation has create a duplicate entry,
            // remove the one at the tail
            activeGameObjects_.pop_back();

            // clear the queried guid from active index and update
            // inactiveGameObjects
            activeIndex_.erase(it);
            inactiveGameObjects_.push_back(guid);

            return true;
        }

        /**
         * @brief Releases and permanently removes a GameObject from the pool.
         *
         * @details Unlike `release()`, this method does not add the Guid back to the
         * inactive list. Use this when a pooled object is being destroyed rather than
         * recycled.
         *
         * @param guid The unique identifier of the GameObject to remove.
         *
         * @return True if removed successfully, false if Guid was not active.
         */
        bool releaseAndRemove(const helios::util::Guid& guid) {

            auto it = activeIndex_.find(guid);

            if (it == activeIndex_.end()) {
                return false;
            }

            const size_t idx = it->second;
            const auto lastGuid = activeGameObjects_.back();

            activeGameObjects_[idx] = lastGuid;
            activeGameObjects_.pop_back();

            activeIndex_.erase(it);

            if (lastGuid != guid) {
                activeIndex_[lastGuid] = idx;
            }

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
