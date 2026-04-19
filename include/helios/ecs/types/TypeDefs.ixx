/**
 * @file TypeDefs.ixx
 * @brief Core type definitions and tag types for the helios engine.
 */
module;

#include <cstdint>
#include <limits>
#include <cstddef>

export module helios.ecs.types.TypeDefs;


export namespace helios::ecs::types {


    /**
     * @brief Version number for detecting stale entity references.
     *
     * Each entity slot in an EntityPool has an associated version that is
     * incremented when the entity is removed. EntityHandles store both the
     * EntityId and VersionId, allowing the pool to detect when a handle
     * refers to a recycled slot.
     *
     * A valid version starts at 1 (uninitialized handles have version 0).
     *
     * @see EntityHandle
     * @see EntityId
     * @see EntityPool
     */
    using VersionId = uint32_t;

    /**
     * @brief Unique identifier for an entity within an EntityPool.
     *
     * Used as the index into the sparse array of an EntityPool. Combined with
     * a VersionId in an EntityHandle, it enables safe entity references that
     * detect stale handles after entity removal.
     *
     * @see EntityHandle
     * @see VersionId
     * @see EntityPool
     */
    using EntityId = uint32_t;


    /**
     * @brief Sentinel value indicating an invalid or removed sparse index.
     *
     * Used in sparse-set data structures (e.g., EntityPool, EntityPool) to mark
     * slots that do not contain valid entity references. When a slot contains this
     * value, the corresponding entity has been removed or was never assigned.
     *
     * @see EntityPool
     * @see EntityPool
     */
    constexpr size_t EntityTombstone = std::numeric_limits<size_t>::max();

}