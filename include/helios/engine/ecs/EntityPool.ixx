/**
 * @file EntityPool.ixx
 * @brief Sparse-set based entity storage with versioned handles.
 */
module;

#include <cassert>
#include <deque>
#include <span>
#include <vector>
#include <memory>


export module helios.engine.ecs.EntityPool;

import helios.engine.ecs.EntityHandle;
import helios.engine.core.data;


export namespace helios::engine::ecs {

    /**
     * @brief Sentinel value indicating an invalid or removed sparse index.
     */
    constexpr size_t tombstone = std::numeric_limits<size_t>::max();

    /**
     * @brief A sparse-set based entity pool with versioned handles.
     *
     * `EntityPool` provides efficient O(1) entity lookup, insertion, and iteration
     * using a sparse-set data structure. Each entity is accessed via an `EntityHandle`
     * that includes a version number to detect stale references after entity removal.
     *
     * **Data Structure:**
     * - `sparse_`: Maps entity IDs to indices in the dense arrays (or `tombstone` if removed).
     * - `dense_`: Stores entity IDs for cache-friendly iteration.
     * - `denseData_`: Stores the actual entity objects (owned via `unique_ptr`).
     * - `version_`: Tracks version numbers per entity ID for handle validation.
     *
     *
     * @tparam Entity The entity type to store. Must be move-constructible.
     *
     * @see EntityHandle
     */
    template<typename Entity>
    class EntityPool {


        /**
         * @brief List that holds free slots in sparse_.
         */
        std::deque<size_t> freeList_;

        /**
         * @brief Maps entity IDs to indices in dense_/denseData_.
         *
         * Contains `tombstone` for removed or unallocated slots.
         */
        std::vector<size_t> sparse_;

        /**
         * @brief Stores entity IDs in a contiguous, cache-friendly layout.
         */
        std::vector<size_t> dense_;

        /**
         * @brief Stores the actual entity objects (ownership via unique_ptr).
         */
        std::vector<std::unique_ptr<Entity>> denseData_;

        /**
         * @brief Tracks version numbers per entity ID for stale handle detection.
         */
        std::vector<size_t> version_;

        /**
         * @brief Resizes sparse_ and version_ arrays if necessary.
         *
         * @param idx The index that must be accessible after resize.
         */
        void resizeIfNecessary(const size_t idx) {
            if (sparse_.size() <= idx) {
                sparse_.resize(idx + 1,tombstone);
            }
            if (version_.size() <= idx) {
                version_.resize(idx + 1, 1);
            }
        }

        /**
         * @brief Retrieves the entity pointer for a given handle.
         *
         * @param handle The handle to look up.
         *
         * @return Pointer to the entity, or nullptr if the handle is invalid or stale.
         */
        [[nodiscard]] Entity* getEntity(const helios::engine::ecs::EntityHandle& handle) const noexcept {
            const auto idx = handle.entityId;
            const auto version = handle.versionId;

            if (idx >= sparse_.size() || sparse_[idx] == tombstone) {
                return nullptr;
            }

            assert(sparse_[idx] < dense_.size() && "Unexpected sparse index");

            if (version_[idx] != version) {
                return nullptr;
            }

            return denseData_[sparse_[idx]].get();

        }



    public:

        /**
         * @brief Default constructor. Creates an empty pool.
         */
        EntityPool() = default;

        /**
         * @brief Constructs an EntityPool with pre-allocated capacity.
         *
         * @param capacity The initial capacity to reserve.
         */
        explicit EntityPool(const size_t capacity) {
            reserve(capacity);
        }


        /**
         * @brief Adds an entity to the pool and returns a handle.
         *
         * Ownership of the entity is transferred to the pool.
         *
         * @param e The entity to add. Ownership is transferred.
         *
         * @return A valid EntityHandle for the newly added entity.
         */
        EntityHandle emplace(std::unique_ptr<Entity> e) {

            size_t idx;

            if (freeList_.empty()) {
                idx = sparse_.size();
            } else {
                idx = freeList_.front();
                freeList_.pop_front();
            }

            resizeIfNecessary(idx);

            sparse_[idx] = dense_.size();
            dense_.push_back(idx);
            denseData_.push_back(std::move(e));

            return {
               .entityId = static_cast<helios::engine::core::data::EntityId>(idx),
                .versionId = static_cast<helios::engine::core::data::VersionId>(version_[idx])
            };

        }


        /**
         * @brief Retrieves an entity by handle.
         *
         * @param handle The handle to look up.
         *
         * @return Pointer to the entity, or nullptr if handle is invalid or stale.
         */
        [[nodiscard]] Entity* get(const EntityHandle& handle) noexcept {
            return getEntity(handle);
        }

        /**
         * @copydoc get(const EntityHandle&)
         */
        [[nodiscard]] const Entity* get(const EntityHandle& handle) const noexcept {
            return getEntity(handle);
        }

        /**
         * @brief Removes an entity from the pool.
         *
         * After removal, any existing handles to this entity become stale
         * (the version is incremented).
         *
         * @param handle The handle of the entity to remove.
         *
         * @note Currently not implemented (asserts false).
         */
        void remove(const EntityHandle& handle) {
            assert(false && "TBD");
            //++version_[idx];
        }



        /**
         * @brief Pre-allocates storage for the specified number of entities.
         *
         * @param capacity The number of entities to reserve space for.
         */
        void reserve(const size_t capacity) {
            sparse_.reserve(capacity);
            dense_.reserve(capacity);
            version_.reserve(capacity);
        }


        /**
         * @brief Returns a span over the version array.
         *
         * @return A mutable span of version numbers.
         */
        std::span<size_t> version() noexcept {
            return version_;
        }

        /**
         * @brief Returns a span over the dense entity ID array.
         *
         * @return A mutable span of entity IDs.
         */
        std::span<size_t> dense() noexcept {
            return dense_;
        }

        /**
         * @brief Returns a span over the sparse mapping array.
         *
         * @return A mutable span of sparse indices.
         */
        std::span<size_t> sparse() noexcept {
            return sparse_;
        }


        /**
         * @brief Returns a span over all stored entities.
         *
         * Provides cache-friendly iteration over all active entities.
         *
         * @return A const span of unique_ptr to the stored entities.
         */
        std::span <const std::unique_ptr<Entity>> entities() const noexcept {
            return denseData_;
        }

        /**
         * @copydoc entities() const
         */
        std::span <std::unique_ptr<Entity>> entities() noexcept {
            return denseData_;
        }
    };



}