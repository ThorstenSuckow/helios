/**
 * @file DistributedSpawnPlacer.ixx
 * @brief Spawn placer that distributes entities across predefined locations.
 */
module;

#include <cassert>
#include <cmath>
#include <algorithm>

export module helios.engine.runtime.spawn.behavior.placements.DistributedSpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.util.Random;
import helios.math;
import helios.util.Guid;

import helios.engine.runtime.world.UpdateContext;


export namespace helios::engine::runtime::spawn::behavior::placements {

    /**
     * @brief Spawn placer that assigns entities to predefined spawn points.
     *
     * @details DistributedSpawnPlacer uses a fixed-size array of locations
     * to distribute spawned entities. Entities are assigned to locations
     * based on their cursor position within the spawn batch, dividing
     * the batch evenly across all available spawn points.
     *
     * ## Distribution Algorithm
     *
     * Entities are mapped to locations using: `bin = N * (position / spawnCount)`
     *
     * This ensures even distribution when spawn count exceeds location count,
     * and direct mapping when spawn count equals location count.
     *
     * ## Use Cases
     *
     * - **Fixed spawn points:** Enemies spawning at predefined positions
     * - **Formation patterns:** Entities placed in specific arrangements
     * - **Level design:** Designer-defined spawn locations
     *
     * ## Usage
     *
     * ```cpp
     * // Create placer with 3 fixed spawn points
     * auto placer = std::make_unique<DistributedSpawnPlacer<3>>(
     *     helios::math::vec3f{-50.0f, 100.0f, 0.0f},
     *     helios::math::vec3f{  0.0f, 100.0f, 0.0f},
     *     helios::math::vec3f{ 50.0f, 100.0f, 0.0f}
     * );
     *
     * // 3 entities spawn at the 3 locations
     * // 6 entities: 2 per location
     * ```
     *
     * @tparam N Number of spawn locations.
     *
     * @see SpawnPlacer
     * @see AxisSpawnPlacer
     */
    template<std::size_t N>
    class DistributedSpawnPlacer final : public SpawnPlacer {

        /**
         * @brief Fixed array of spawn point locations.
         */
        std::array<helios::math::vec3f, N> locations_{};

    public:

        /**
         * @brief Constructs a DistributedSpawnPlacer with N spawn locations.
         *
         * @tparam Args Types of location arguments (must be vec3f).
         *
         * @param args Exactly N spawn point locations.
         *
         * @note The number of arguments must match template parameter N.
         */
        template<typename... Args>
        requires(sizeof...(Args) == N)
        explicit DistributedSpawnPlacer(Args&& ...args) : locations_({std::forward<Args>(args)... }) {}

        /**
         * @brief Returns the spawn location for the current entity.
         *
         * @details Maps the cursor position to one of the predefined locations
         * by dividing the spawn batch evenly across available spawn points.
         *
         * @param guid Unique identifier of the entity (unused).
         * @param gameObjectBounds Bounding box of the entity (unused).
         * @param environmentBounds Bounding box of the environment (unused).
         * @param cursor Current spawn batch cursor with position and count.
         * @param spawnContext Context for the spawn operation (unused).
         *
         * @return World position from the locations array.
         */
        helios::math::vec3f getPosition(
            const helios::util::Guid& guid,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            const size_t bin = N * (cursor.position/static_cast<float>(cursor.spawnCount));

            return locations_[bin];
        }


    };

}