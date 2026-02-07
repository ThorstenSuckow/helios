/**
 * @file EntityRegistry.ixx
 * @brief Defines the EntityRegistry class for managing entity lifecycles.
 */

module;

#include <span>
#include <vector>
#include <memory>


export module helios.engine.ecs.EntityRegistry;

import helios.engine.ecs.EntityHandle;
import helios.engine.core.data;


export namespace helios::engine::ecs {


    /**
     * @brief The initial version assigned to newly created entities.
     *
     * Versions start at 1 to distinguish valid handles from default-initialized handles
     * that may have a version of 0.
     */
    constexpr auto InitialVersion = helios::engine::core::data::VersionId{1};

    /**
     * @brief Central registry for creating and managing entity handles.
     *
     * The `EntityRegistry` serves as the authoritative source for entity lifecycle
     * management in the ECS architecture. It is responsible for:
     *
     * - **Handle Creation:** Generates unique `EntityHandle` instances with versioned IDs.
     * - **Validation:** Determines whether a given handle refers to a currently alive entity.
     * - **Destruction:** Marks entities as destroyed by incrementing their version and
     *   recycling the index for future use.
     *
     * ## Versioning
     *
     * Each entity slot maintains a version number. When an entity is destroyed, its
     * version is incremented. This allows the registry to detect **stale handles** —
     * handles that reference an entity that has been destroyed and potentially replaced
     * by a new entity at the same index.
     *
     * ## Index Recycling
     *
     * Destroyed entity indices are added to a free list and reused when creating new
     * entities. This keeps the dense version array compact and minimizes memory growth.
     *
     * ## Usage Example
     *
     * ```cpp
     * helios::engine::ecs::EntityRegistry registry;
     *
     * // Create a new entity
     * auto handle = registry.create();
     *
     * // Check if the handle is valid
     * if (registry.isValid(handle)) {
     *     // Entity is alive, safe to use
     * }
     *
     * // Destroy the entity
     * registry.destroy(handle);
     *
     * // The handle is now stale
     * assert(!registry.isValid(handle));
     * ```
     *
     * @see EntityHandle
     * @see helios::engine::core::data::EntityId
     * @see helios::engine::core::data::VersionId
     */
    class EntityRegistry {

        /**
         * @brief Free list of recycled entity indices available for reuse.
         */
        std::vector<helios::engine::core::data::EntityId> freeList_;

        /**
         * @brief Version numbers for each entity slot.
         *
         * The version is incremented when an entity at that index is destroyed,
         * allowing detection of stale handles.
         */
        std::vector<helios::engine::core::data::VersionId> versions_;

    public:

        /**
         * @brief Default constructor. Creates an empty registry.
         */
        EntityRegistry() = default;

        /**
         * @brief Constructs an EntityRegistry with pre-allocated capacity.
         *
         * Reserving capacity upfront can improve performance when the approximate
         * number of entities is known in advance.
         *
         * @param capacity The initial capacity to reserve for the version vector.
         */
        explicit EntityRegistry(const size_t capacity) {
            versions_.reserve(capacity);
        }

        /**
         * @brief Creates a new entity and returns its handle.
         *
         * If recycled indices are available in the free list, one is reused.
         * Otherwise, a new index is allocated at the end of the version vector.
         *
         * @return A valid `EntityHandle` for the newly created entity.
         */
        EntityHandle create() {

            helios::engine::core::data::EntityId idx;
            helios::engine::core::data::VersionId version;

            if (freeList_.empty()) {
                idx = versions_.size();
                version = InitialVersion;

                versions_.push_back(version);

            } else {
                idx = freeList_.back();
                freeList_.pop_back();

                // version was already incremented in destroy
                version = versions_[idx];
            }


            return {idx, version};

        }

        /**
         * @brief Checks whether the given handle refers to a currently alive entity.
         *
         * A handle is valid if its index is within bounds and its version matches
         * the current version stored in the registry.
         *
         * @param handle The handle to validate.
         *
         * @return `true` if the handle is valid and the entity is alive,
         *         `false` otherwise.
         */
        [[nodiscard]] bool isValid(const EntityHandle handle) const noexcept {

            const auto index = handle.entityId;

            if (index >= versions_.size()) {
                return false;
            }

            return handle.versionId == versions_[index];
        }

        /**
         * @brief Destroys the entity referenced by the given handle.
         *
         * If the handle is valid, the entity's version is incremented (invalidating
         * all existing handles to it) and its index is added to the free list for
         * recycling.
         *
         * @param handle The handle of the entity to destroy.
         *
         * @return `true` if the entity was successfully destroyed,
         *         `false` if the handle was already invalid.
         */
        bool destroy(const EntityHandle handle) {

            if (!isValid(handle)) {
                return false;
            }

            const auto index = handle.entityId;

            versions_[index] += 1;

            freeList_.push_back(index);

            return true;
        }

        /**
         * @brief Returns a view of the version vector.
         *
         * Provides read-write access to the internal version data. Useful for
         * debugging or advanced use cases where direct version inspection is needed.
         *
         * @return A span over the version vector.
         */
        std::span<helios::engine::core::data::VersionId> version() noexcept {
            return versions_;
        }

    };



}