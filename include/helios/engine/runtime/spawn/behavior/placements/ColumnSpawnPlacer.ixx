/**
 * @file ColumnSpawnPlacer.ixx
 * @brief Spawn placer that arranges entities in a vertical column.
 */
module;

#include <cmath>

export module helios.engine.runtime.spawn.behavior.placements.ColumnSpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.util.Random;
import helios.math;
import helios.util.Guid;

import helios.engine.runtime.world.UpdateContext;


export namespace helios::engine::runtime::spawn::behavior::placements {

    /**
     * @brief Places spawned entities in a vertical column formation.
     *
     * Distributes entities evenly along the Y-axis within the environment
     * bounds. Entities are positioned at the left edge (min X) of the
     * environment and spaced vertically based on the spawn count.
     *
     * For single spawns, a random Y position is chosen within bounds.
     * For multiple spawns, entities are evenly distributed from top to bottom.
     *
     * Example layout for 4 entities:
     * ```
     * ┌─────────────────────┐
     * │ ○                   │  <- position 1
     * │                     │
     * │ ○                   │  <- position 2
     * │                     │
     * │ ○                   │  <- position 3
     * │                     │
     * │ ○                   │  <- position 4
     * └─────────────────────┘
     * ```
     */
    class ColumnSpawnPlacer final : public SpawnPlacer {

    public:

        /**
         * @brief Calculates spawn position for a vertical column layout.
         *
         * @param guid The GUID of the entity being spawned.
         * @param gameObjectBounds The AABB of the entity to spawn.
         * @param environmentBounds The AABB of the spawn environment (arena).
         * @param cursor Current position in the spawn batch.
         * @param spawnContext Additional context for spawn decisions.
         *
         * @return World position for the entity (left edge, vertically distributed).
         */
        helios::math::vec3f getPosition(
            const helios::util::Guid& guid,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            const float top    = environmentBounds.max()[1];
            const float bottom = environmentBounds.min()[1];
            const float height = std::abs(top - bottom);

            const float objectHeight = gameObjectBounds.size()[1];

            size_t position = cursor.position;

            float dist;
            if (cursor.spawnCount == 1) {
                dist = helios::util::Random(12345).randomFloat(0, height - objectHeight);
                position = 1;
            } else {
                dist = height / static_cast<float>(cursor.spawnCount);
            }

            const float x = environmentBounds.min()[0];
            const float y = top - (static_cast<float>(position) * dist) - (objectHeight / 2.0f);

            return helios::math::vec3f{x, y, 0.0f};

        }


    };

}