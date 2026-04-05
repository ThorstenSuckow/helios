/**
 * @file types.ixx
 * @brief Core type definitions and tag types for the helios engine.
 */
module;

#include <cstdint>
#include <limits>

export module helios.core.types;


export namespace helios::core::types {

    /**
     * @brief Tag type used to indicate skipping default initialization.
     *
     * @details `no_init_t` is a tag type that signals to constructors or
     * factory functions that default initialization should be skipped.
     * This is useful for performance-critical code paths where objects
     * will be immediately overwritten or initialized manually.
     *
     * ## Usage
     *
     * Pass `no_init` as a constructor argument to request uninitialized storage:
     *
     * ```cpp
     * import helios.util.Guid;
     * import helios.core.types;
     *
     * // Generate a new unique Guid
     * auto id = helios::util::Guid::generate();
     *
     * // Declare uninitialized Guid for later assignment
     * helios::util::Guid deferredId{helios::core::types::no_init};
     *
     * // Assign later
     * deferredId = helios::util::Guid::generate();
     * ```
     *
     * @note Objects constructed with `no_init` are in an indeterminate state.
     * Reading from them before assignment is undefined behavior.
     *
     * @see no_init
     * @see helios::util::Guid
     */
    struct no_init_t{};

    /**
     * @brief Tag constant for requesting uninitialized construction.
     *
     * @details This is a convenience instance of `no_init_t` that can be
     * passed to constructors supporting uninitialized construction.
     *
     * ```cpp
     * helios::util::Guid id{helios::core::types::no_init};
     * ```
     *
     * @see no_init_t
     * @see helios::util::Guid
     */
    inline constexpr no_init_t no_init;

    /**
     * @brief Underlying integer type for all strong identifiers.
     *
     * @details `StrongId_t` is the common numeric representation used by
     * `StrongId<Tag>` and related lookup structures (e.g., `EntityRegistry`,
     * `LinearLookupStrategy`, `HashedLookupStrategy`). Centralising the
     * typedef ensures a consistent width across the entire identifier
     * subsystem.
     *
     * @see helios::core::data::StrongId
     * @see EntityId
     * @see VersionId
     */
    using StrongId_t = uint32_t;

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
     * Used in sparse-set data structures (e.g., EntityPool, GameObjectPool) to mark
     * slots that do not contain valid entity references. When a slot contains this
     * value, the corresponding entity has been removed or was never assigned.
     *
     * @see EntityPool
     * @see GameObjectPool
     */
    constexpr size_t EntityTombstone = std::numeric_limits<size_t>::max();
}