/**
 * @file EntityHandle.ixx
 * @brief Versioned, strongly-typed handle for referencing entities.
 */
module;


#include <functional>
#include <cstdint>

export module helios.ecs.types.EntityHandle;

import helios.ecs.types.TypeDefs;
import helios.ecs.concepts.IsStrongIdLike;

using namespace helios::ecs::concepts;
export namespace helios::ecs::types {

    /**
     * @brief Sentinel version indicating an invalid or uninitialized handle.
     */
    constexpr auto InvalidVersion = VersionId{0};

    /**
     * @brief The initial version assigned to newly created entities.
     *
     * Versions start at 1 to distinguish valid handles from default-initialized
     * handles that may have a version of 0.
     */
    constexpr auto InitialVersion = VersionId{1};


    /**
     * @brief A versioned handle for referencing entities in a registry.
     *
     * `EntityHandle` combines an entity ID with a version number and a
     * domain-specific strong ID to provide safe, typed references to
     * registered entities. The version ensures that handles to removed
     * entities are detected as stale.
     *
     * @tparam TStrongId A strong ID type carrying domain semantics.
     *
     * @see EntityRegistry
     */
    template<typename TStrongId>
    requires IsStrongIdLike<TStrongId>
    struct EntityHandle {

        /**
         * @brief The unique identifier for the entity within the registry.
         */
        EntityId entityId{0};

        /**
         * @brief The version number for stale handle detection.
         *
         * Incremented when an entity is removed from the registry.
         */
        VersionId versionId = InvalidVersion;

        /**
         * @brief The domain-specific strong ID associated with this handle.
         */
        TStrongId strongId{};

        /**
         * @brief Compares two handles for equality.
         */
        bool operator==(const EntityHandle&) const = default;

        /**
         * @brief Compares two handles by entity ID (less-than).
         *
         * @param entityHandle The handle to compare against.
         *
         * @return True if this handle's entity ID is less than the other.
         */
        constexpr bool operator<(const EntityHandle& entityHandle) const noexcept {
            return entityId < entityHandle.entityId;
        }

        /**
         * @brief Compares two handles by entity ID (greater-than).
         *
         * @param entityHandle The handle to compare against.
         *
         * @return True if this handle's entity ID is greater than the other.
         */
        constexpr bool operator>(const EntityHandle& entityHandle) const noexcept {
            return entityId > entityHandle.entityId;
        }


        /**
         * @brief Checks if this handle is potentially valid.
         *
         * @return True if the version is at least 1 (initial version).
         */
        [[nodiscard]] bool isValid() const noexcept {
            return versionId >= 1;
        }

    };

}


/**
 * @brief Hash specialization for EntityHandle.
 *
 * Packs entityId and versionId into a 64-bit value for hashing.
 */
template<typename TStrongId>
struct std::hash<helios::ecs::types::EntityHandle<TStrongId>> {
    std::size_t operator()(const helios::ecs::types::EntityHandle<TStrongId>& handle) const noexcept {

        const uint64_t packed = (static_cast<uint64_t>(handle.entityId) << 32) |
                                static_cast<uint64_t>(handle.versionId);

        return std::hash<uint64_t>{}(packed);
    }
};
