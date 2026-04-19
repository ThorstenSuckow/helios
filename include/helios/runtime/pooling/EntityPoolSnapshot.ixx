/**
 * @file EntityPoolSnapshot.ixx
 * @brief Read-only snapshot of EntityPool state.
 */
module;


#include <cstddef>
export module helios.runtime.pooling.EntityPoolSnapshot;

export namespace helios::runtime::pooling {

    /**
     * @brief Immutable snapshot of a EntityPool's current state.
     *
     * @details EntityPoolSnapshot provides a point-in-time view of a pool's
     * active and inactive entity counts. This is useful for monitoring pool
     * utilization, debugging spawn issues, and making spawn decisions based
     * on available capacity.
     *
     * The snapshot is read-only and captures the state at creation time;
     * subsequent pool operations do not affect it.
     *
     * Example usage:
     * ```cpp
     * auto snapshot = poolManager.poolSnapshot(bulletPoolId);
     * if (snapshot.inactiveCount > 0) {
     *     // Pool has available capacity for spawning
     * }
     * ```
     *
     * @see EntityPool
     * @see EntityPoolManager::poolSnapshot
     */
    struct EntityPoolSnapshot {

        /**
         * @brief Number of currently active (in-use) entities in the pool.
         */
        const size_t activeCount;

        /**
         * @brief Number of currently inactive (available) entities in the pool.
         */
        const size_t inactiveCount;

    };

}