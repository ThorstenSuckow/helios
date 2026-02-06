/**
 * @file SpawnPlacer.ixx
 * @brief Abstract interface for determining spawn positions.
 */
module;

#include <optional>

export module helios.engine.runtime.spawn.behavior.SpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.math;
import helios.engine.ecs.EntityHandle;

export namespace helios::engine::runtime::spawn::behavior {

    /**
     * @brief Abstract interface for determining spawn positions.
     *
     * @details SpawnPlacer defines the strategy for calculating where a spawned
     * entity should be placed in the world. Implementations can provide various
     * placement strategies:
     *
     * - **RandomPlacer:** Random position within level bounds
     * - **FormationPlacer:** Grid or pattern-based placement
     * - **EmitterRelativePlacer:** Offset from emitter position
     * - **FixedPlacer:** Predetermined spawn points
     *
     * Example implementation:
     * ```cpp
     * class RandomPlacer : public SpawnPlacer {
     * public:
     *     vec3f getPosition(const Guid& guid, const aabbf& bounds,
     *                       const SpawnPlanCursor& cursor,
     *                       const SpawnContext& ctx) override {
     *         return Random::pointInBounds(bounds);
     *     }
     * };
     * ```
     *
     * @see SpawnProfile
     * @see SpawnInitializer
     * @see SpawnContext
     */
    class SpawnPlacer {

    public:

        virtual ~SpawnPlacer() = default;

        /**
         * @brief Calculates the spawn position for an entity.
         *
         * @param entityHandle The handle of the entity being spawned.
         * @param gameObjectBounds The bounding box of the entity to spawn.
         * @param environmentBounds The level bounds to spawn within.
         * @param cursor The current position within the spawn batch.
         * @param spawnContext Context data including optional emitter info.
         *
         * @return The world position for the spawned entity.
         */
        virtual helios::math::vec3f getPosition(
            const helios::engine::ecs::EntityHandle& entityHandle,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) = 0;


    };

}