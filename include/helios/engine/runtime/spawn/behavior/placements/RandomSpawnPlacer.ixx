/**
 * @file RandomSpawnPlacer.ixx
 * @brief Placer that spawns entities at random positions within bounds.
 */
module;

export module helios.engine.runtime.spawn.behavior.placements.RandomSpawnPlacer;

import helios.engine.runtime.spawn.SpawnPlanCursor;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.util.Random;
import helios.math;
import helios.engine.ecs.EntityHandle;

export namespace helios::engine::runtime::spawn::behavior::placements {

    /**
     * @brief Placer that spawns entities at random positions within bounds.
     *
     * @details RandomSpawnPlacer generates random X and Y coordinates within
     * the provided level bounds. The Z coordinate is set to 0, making this
     * suitable for 2D games.
     *
     * @note Uses a static Random generator with a fixed seed for reproducibility.
     *
     * @see SpawnPlacer
     */
    class RandomSpawnPlacer final : public SpawnPlacer {

    public:

        /**
         * @brief Returns a random position within the level bounds.
         *
         * @param entityHandle The handle of the entity being spawned (unused).
         * @param gameObjectBounds The bounding box of the entity to spawn (unused).
         * @param environmentBounds The level bounds to spawn within.
         * @param cursor The current position within the spawn batch (unused).
         * @param spawnContext Context data (unused).
         *
         * @return A random vec3f with Z = 0.
         */
        helios::math::vec3f getPosition(
            const helios::engine::ecs::EntityHandle& entityHandle,
            const helios::math::aabbf& gameObjectBounds,
            const helios::math::aabbf& environmentBounds,
            const SpawnPlanCursor& cursor,
            const SpawnContext& spawnContext
        ) override {

            static auto rGen = helios::util::Random(12345);

            float xPos = rGen.randomFloat(environmentBounds.min()[0], environmentBounds.max()[0]);
            float yPos = rGen.randomFloat(environmentBounds.min()[1], environmentBounds.max()[1]);

            return helios::math::vec3f{xPos, yPos, 0.0f};
        }


    };

}