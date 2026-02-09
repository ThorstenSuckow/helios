/**
 * @file EntityId.ixx
 * @brief Type alias for entity identifiers within an EntityPool.
 */
module;

#include <cstdint>

export module helios.engine.core.data.EntityId;



export namespace helios::engine::core::data {

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

}