/**
 * @file EntityTombstone.ixx
 * @brief Sentinel value for invalid sparse array indices.
 */
module;


#include <limits>


export module helios.engine.core.data.EntityTombstone;


export namespace helios::engine::core::data {

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
