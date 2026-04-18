/**
 * @file GameObjectPoolRegistry.ixx
 * @brief Central registry for managing multiple GameObjectPool instances.
 */
module;

#include <memory>
#include <unordered_map>
#include <ranges>

export module helios.runtime.pooling.GameObjectPoolRegistry;


import helios.runtime.pooling.GameObjectPool;
import helios.runtime.pooling.types.GameObjectPoolId;


export namespace helios::runtime::pooling {

    /**
     * @brief Central registry for managing multiple named GameObjectPools.
     *
     * @details GameObjectPoolRegistry provides a single point of access to all object
     * pools in the game. Each pool is identified by a strongly-typed GameObjectPoolId,
     * enabling type-safe lookup without string comparisons.
     *
     * The registry owns all pools exclusively via unique_ptr, ensuring proper cleanup
     * when the registry is destroyed.
     *
     * Example usage:
     * ```cpp
     * GameObjectPoolRegistry registry;
     *
     * constexpr GameObjectPoolId BULLET_POOL{1};
     * registry.addPool(BULLET_POOL, std::make_unique<GameObjectPool>(100));
     *
     * // Later, acquire a bullet from the pool
     * auto* pool = registry.pool(BULLET_POOL);
     * helios::util::Guid bulletGuid;
     * if (pool && pool->acquire(bulletGuid)) {
     *     // Use the bullet
     * }
     * ```
     */
    template<typename THandle>
    class GameObjectPoolRegistry {

    private:

        /**
         * @brief Maps pool IDs to their corresponding GameObjectPool instances.
         */
        std::unordered_map<helios::runtime::pooling::types::GameObjectPoolId, std::unique_ptr<GameObjectPool<THandle>>> pools_;

    public:

        /**
         * @brief Default constructor.
         */
        GameObjectPoolRegistry() = default;

        /**
         * @brief Adds a new pool to the registry.
         *
         * @details If a pool with the given ID already exists, it will be replaced.
         * Ownership of the pool is transferred to the registry.
         *
         * @param id The unique identifier for this pool.
         * @param gameObjectPool The pool to add (ownership is transferred).
         *
         * @return Raw pointer to the added pool for immediate use.
         */
        GameObjectPool<THandle>* addPool(
            const helios::runtime::pooling::types::GameObjectPoolId id,
            std::unique_ptr<GameObjectPool<THandle>> gameObjectPool
        ) noexcept {
            pools_[id] = std::move(gameObjectPool);
            return pools_[id].get();
        }

        /**
         * @brief Returns a reference to the internal pool map.
         *
         * @details Provides direct access to all pools for iteration or bulk operations.
         * The returned map should not be modified directly; use `addPool()` instead.
         *
         * @return Reference to the pool map.
         */
        [[nodiscard]]std::unordered_map<helios::runtime::pooling::types::GameObjectPoolId, std::unique_ptr<GameObjectPool<THandle>>>& pools() {
            return pools_;
        }


        /**
         * @brief Retrieves a pool by its ID.
         *
         * @param id The identifier of the pool to retrieve.
         *
         * @return Pointer to the pool, or nullptr if not found.
         */
        [[nodiscard]] GameObjectPool<THandle>* pool(const helios::runtime::pooling::types::GameObjectPoolId id) const {

            const auto& it = pools_.find(id);

            if (it == pools_.end()) {
                return nullptr;
            }

            return it->second.get();
        }

        /**
         * @brief Checks if a pool with the given ID is registered.
         *
         * @param id The identifier of the pool to check.
         *
         * @return True if the pool exists, false otherwise.
         */
        [[nodiscard]] bool has(const helios::runtime::pooling::types::GameObjectPoolId id) const noexcept {
            return pools_.contains(id);
        }


    };

}
