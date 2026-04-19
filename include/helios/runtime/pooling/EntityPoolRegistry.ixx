/**
 * @file EntityPoolRegistry.ixx
 * @brief Central registry for managing multiple EntityPool instances.
 */
module;

#include <memory>
#include <unordered_map>
#include <ranges>

export module helios.runtime.pooling.EntityPoolRegistry;


import helios.runtime.pooling.EntityPool;
import helios.runtime.pooling.types.EntityPoolId;


export namespace helios::runtime::pooling {

    /**
     * @brief Central registry for managing multiple named EntityPools.
     *
     * @details EntityPoolRegistry provides a single point of access to all object
     * pools in the game. Each pool is identified by a strongly-typed EntityPoolId,
     * enabling type-safe lookup without string comparisons.
     *
     * The registry owns all pools exclusively via unique_ptr, ensuring proper cleanup
     * when the registry is destroyed.
     *
     * Example usage:
     * ```cpp
     * EntityPoolRegistry registry;
     *
     * constexpr EntityPoolId BULLET_POOL{1};
     * registry.addPool(BULLET_POOL, std::make_unique<EntityPool>(100));
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
    class EntityPoolRegistry {

    private:

        /**
         * @brief Maps pool IDs to their corresponding EntityPool instances.
         */
        std::unordered_map<helios::runtime::pooling::types::EntityPoolId, std::unique_ptr<EntityPool<THandle>>> pools_;

    public:

        /**
         * @brief Default constructor.
         */
        EntityPoolRegistry() = default;

        /**
         * @brief Adds a new pool to the registry.
         *
         * @details If a pool with the given ID already exists, it will be replaced.
         * Ownership of the pool is transferred to the registry.
         *
         * @param id The unique identifier for this pool.
         * @param entityPool The pool to add (ownership is transferred).
         *
         * @return Raw pointer to the added pool for immediate use.
         */
        EntityPool<THandle>* addPool(
            const helios::runtime::pooling::types::EntityPoolId id,
            std::unique_ptr<EntityPool<THandle>> entityPool
        ) noexcept {
            pools_[id] = std::move(entityPool);
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
        [[nodiscard]]std::unordered_map<helios::runtime::pooling::types::EntityPoolId, std::unique_ptr<EntityPool<THandle>>>& pools() {
            return pools_;
        }


        /**
         * @brief Retrieves a pool by its ID.
         *
         * @param id The identifier of the pool to retrieve.
         *
         * @return Pointer to the pool, or nullptr if not found.
         */
        [[nodiscard]] EntityPool<THandle>* pool(const helios::runtime::pooling::types::EntityPoolId id) const {

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
        [[nodiscard]] bool has(const helios::runtime::pooling::types::EntityPoolId id) const noexcept {
            return pools_.contains(id);
        }


    };

}
