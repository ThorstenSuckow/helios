/**
 * @file SpawnPlanCursor.ixx
 * @brief Cursor tracking progress within a spawn batch.
 */
module;

export module helios.engine.runtime.spawn.SpawnPlanCursor;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Tracks the current position within a spawn batch.
     *
     * @details SpawnPlanCursor provides context to SpawnPlacer and SpawnInitializer
     * about where the current spawn is within its batch. This enables behaviors
     * like distributing spawns in a formation or applying sequential variations.
     *
     * Example:
     * ```cpp
     * // Spawning 5 enemies in a row
     * for (size_t i = 0; i < spawnCount; ++i) {
     *     SpawnPlanCursor cursor{.spawnCount = spawnCount, .position = i};
     *     auto pos = placer->getPosition(guid, bounds, cursor, ctx);
     * }
     * ```
     *
     * @see SpawnPlacer
     * @see SpawnInitializer
     */
    struct SpawnPlanCursor {

        /**
         * @brief Total number of entities to spawn in this batch.
         */
        const size_t spawnCount;

        /**
         * @brief Zero-based index of the current spawn within the batch.
         */
        const size_t position;

    };

}