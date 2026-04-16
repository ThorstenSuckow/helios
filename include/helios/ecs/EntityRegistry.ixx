/**
 * @file EntityRegistry.ixx
 * @brief Generic, policy-based entity registry for managing entity lifecycles.
 */
module;

#include "helios/helios_config.h"
#include <vector>
#include <cassert>

export module helios.ecs.EntityRegistry;

import helios.core.types.TypeDefs;
import helios.ecs.types.TypeDefs;
import helios.ecs.types.EntityHandle;
import helios.ecs.strategies.LinearLookupStrategy;
import helios.ecs.strategies.HashedLookupStrategy;

import helios.ecs.concepts;

using namespace helios::ecs::types;
using namespace helios::core::types;
using namespace helios::ecs::strategies;
export namespace helios::ecs {


    /**
     * @brief Generic registry for creating and managing strongly-typed entity handles.
     *
     * `EntityRegistry` serves as the authoritative source for entity lifecycle
     * management. It is responsible for:
     *
     * - **Handle Creation:** Generates unique `EntityHandle` instances with versioned IDs
     *   and domain-specific strong IDs.
     * - **Validation:** Determines whether a given handle refers to a currently alive entity.
     * - **Destruction:** Marks entities as destroyed by incrementing their version and
     *   recycling the index for future use (when `TAllowRemoval` is true).
     *
     * ## Versioning
     *
     * Each entity slot maintains a version number. When an entity is destroyed, its
     * version is incremented. This allows the registry to detect stale handles —
     * handles that reference an entity that has been destroyed and potentially replaced
     * by a new entity at the same index.
     *
     * ## Index Recycling
     *
     * Destroyed entity indices are added to a free list and reused when creating new
     * entities. This keeps the dense version array compact and minimizes memory growth.
     *
     * ## Strong ID Collision Detection
     *
     * The registry delegates collision checks to a configurable `TLookupStrategy`,
     * which tracks which strong IDs are currently in use. The default strategy
     * (`HashedLookupStrategy`) provides O(1) amortized lookups.
     *
     * @tparam TStrongId       A strong ID type carrying domain semantics.
     * @tparam TLookupStrategy The strategy used for strong ID collision detection.
     * @tparam TAllowRemoval   If false, `destroy()` triggers an assertion instead of removing.
     * @tparam TCapacity       Default initial capacity for pre-allocation.
     *
     * @see EntityHandle
     * @see HashedLookupStrategy
     * @see LinearLookupStrategy
     */
    template<
        typename TStrongId,
        typename TLookupStrategy = HashedLookupStrategy,
        bool TAllowRemoval = true,
        size_t TCapacity = DEFAULT_ENTITY_MANAGER_CAPACITY
    >
    requires helios::ecs::concepts::IsStrongIdLike<TStrongId> &&
    helios::ecs::concepts::IsStrongIdCollisionResolverLike<TLookupStrategy>
    class EntityRegistry {


        /**
         * @brief Free list of recycled entity indices available for reuse.
         */
        std::vector<EntityId> freeList_;


        /**
         * @brief Version numbers for each entity slot.
         *
         * The version is incremented when an entity at that index is destroyed,
         * allowing detection of stale handles.
         */
        std::vector<VersionId> versions_;
        

        /**
         * @brief Strong ID values for each entity slot.
         */
        std::vector<StrongId_t> strongIds_;

        /**
         * @brief Lookup strategy instance for strong ID collision detection.
         */
        TLookupStrategy lookupStrategy_;

        /**
         * @brief Auto-increment counter for generating strong IDs when none is provided.
         */
        size_t strongIdCounter_ = 0;

    public:

        /**
         * @brief Constructs a registry with pre-allocated capacity.
         *
         * Reserving capacity upfront can improve performance when the approximate
         * number of entities is known in advance.
         *
         * @param capacity The initial capacity to reserve.
         */
        explicit EntityRegistry(const size_t capacity = TCapacity)
          : lookupStrategy_(capacity) {
            versions_.reserve(capacity);
            strongIds_.reserve(capacity);
            freeList_.reserve(capacity);
        }


        /**
         * @brief Creates a new entity and returns its handle.
         *
         * If recycled indices are available in the free list, one is reused.
         * Otherwise, a new index is allocated at the end of the version vector.
         * If the provided `strongId` is invalid, one is auto-assigned from the
         * entity index.
         *
         * @param strongId Optional domain-specific strong ID. Defaults to auto-assignment.
         *
         * @return A valid `EntityHandle` for the newly created entity.
         */
        EntityHandle<TStrongId> create(TStrongId strongId = TStrongId{}) {

            EntityId idx;
            VersionId version;

            if (freeList_.empty()) {
                idx = static_cast<EntityId>(versions_.size());
                version = InitialVersion;
                versions_.push_back(version);
                strongIds_.push_back(StrongId_t{});
            } else {
                idx = freeList_.back();
                freeList_.pop_back();

                // version was already incremented in destroy
                version = versions_[idx];
            }

            if (!strongId.isValid()) {
                strongId = TStrongId{static_cast<StrongId_t>(++strongIdCounter_)};
            }
            assert(strongId.isValid() && "EntityRegistry: invalid strongId");

            assert(!lookupStrategy_.has(strongId.value()) && "EntityRegistry: strongId collision");

            strongIds_[idx] = strongId.value();

            const bool added = lookupStrategy_.add(strongId.value());

            assert(added && "EntityRegistry: failed to add strongId to lookupStrategy");

            return {idx, version, strongId};

        }

        /**
         * @brief Looks up the version for an EntityId.
         *
         * @param entityId The entity to retrieve the version for.
         *
         * @return The version for the EntityId, or `InvalidVersion` if not found.
         */
        [[nodiscard]] VersionId version(const EntityId entityId) const {
            if (entityId >= static_cast<EntityId>(versions_.size())) {
                return InvalidVersion;
            }
            return versions_[entityId];
        }

        /**
         * @brief Looks up the strong ID for an entity index.
         *
         * @param entityId The entity index to retrieve the strong ID for.
         *
         * @return The strong ID for the entity, or a default-constructed
         *         (invalid) `TStrongId` if out of bounds.
         */
        [[nodiscard]] TStrongId strongId(const EntityId entityId) const {
            if (entityId >= static_cast<EntityId>(strongIds_.size())) {
                return TStrongId{};
            }
            return static_cast<TStrongId>(strongIds_[entityId]);
        }


        /**
         * @brief Checks whether the given handle refers to a currently alive entity.
         *
         * A handle is valid if its index is within bounds, its version matches
         * the current version stored in the registry, and its strong ID matches
         * the registered value.
         *
         * @param handle The handle to validate.
         *
         * @return True if the handle is valid and the entity is alive.
         */
        [[nodiscard]] bool isValid(const EntityHandle<TStrongId> handle) const noexcept {
            const auto index = handle.entityId;

            if (index >= static_cast<EntityId>(versions_.size())) {
                return false;
            }

            return handle.strongId.value() == strongIds_[index] &&
                   handle.versionId == versions_[index] &&
                   handle.strongId.isValid();
        }


        /**
         * @brief Destroys the entity referenced by the given handle.
         *
         * If the handle is valid, the entity's version is incremented (invalidating
         * all existing handles to it), the strong ID is unregistered, and the index
         * is added to the free list for recycling.
         *
         * If `TAllowRemoval` is false, triggers an assertion failure.
         *
         * @param handle The handle of the entity to destroy.
         *
         * @return True if the entity was successfully destroyed, false if the handle
         *         was already invalid.
         */
        bool destroy(const EntityHandle<TStrongId> handle) {

            if constexpr (!TAllowRemoval) {
                assert(false && "EntityRegistry: Entity removal is not allowed");
                return false;
            }

            if (!isValid(handle)) {
                return false;
            }

            const auto index = handle.entityId;

            versions_[index] += 1;
            strongIds_[index] = 0;

            const bool removed = lookupStrategy_.remove(handle.strongId.value());
            assert(removed && "EntityRegistry: failed to remove strongId from lookupStrategy");

            freeList_.push_back(index);

            return true;
        }

    };



}