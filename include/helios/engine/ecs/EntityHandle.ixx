/**
 * @file EntityHandle.ixx
 * @brief Versioned handle for referencing entities in an EntityPool.
 */
module;


#include <functional>

export module helios.engine.ecs.EntityHandle;

import helios.engine.core.data;



export namespace helios::engine::ecs {

    /**
     * @brief A versioned handle for referencing entities in an EntityPool.
     *
     * `EntityHandle` combines an entity ID with a version number to provide
     * safe references to pooled entities. The version ensures that handles
     * to removed entities are detected as stale.
     *
     * @see EntityPool
     */
    struct EntityHandle {

        /**
         * @brief The unique identifier for the entity within the pool.
         */
        helios::engine::core::data::EntityId entityId;

        /**
         * @brief The version number for stale handle detection.
         *
         * Incremented when an entity is removed from the pool.
         */
        helios::engine::core::data::VersionId versionId;

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
template<>
struct std::hash<helios::engine::ecs::EntityHandle> {
    std::size_t operator()(const helios::engine::ecs::EntityHandle& handle) const noexcept {

        const uint64_t packed = (static_cast<uint64_t>(handle.entityId) << 32) |
                                static_cast<uint64_t>(handle.versionId);

        return std::hash<uint64_t>{}(packed);
    }
};

