/**
 * @file VersionId.ixx
 * @brief Type alias for entity version numbers used in handle validation.
 */
module;

#include <cstdint>

export module helios.engine.core.data.VersionId;



export namespace helios::engine::core::data {

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

}