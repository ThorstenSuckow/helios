/**
 * @file SpawnContext.ixx
 * @brief Context data passed to spawn behaviors during entity initialization.
 */
module;

#include <optional>

export module helios.engine.runtime.spawn.SpawnContext;

import helios.engine.runtime.spawn.EmitterContext;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Context data passed to spawn behaviors during entity initialization.
     *
     * @details SpawnContext aggregates all contextual information needed by
     * SpawnPlacer and SpawnInitializer to configure a spawned entity. It may
     * contain an EmitterContext if the spawn was triggered by another entity.
     *
     * Example usage:
     * ```cpp
     * SpawnContext ctx;
     * ctx.emitterContext = EmitterContext{shipPos, shipVelocity};
     *
     * placer->getPosition(guid, bounds, cursor, ctx);
     * initializer->initialize(entity, cursor, ctx);
     * ```
     *
     * @see EmitterContext
     * @see SpawnPlacer
     * @see SpawnInitializer
     */
    struct SpawnContext {

        /**
         * @brief Optional context from the entity that triggered the spawn.
         *
         * Present when an entity (e.g., spaceship) triggered the spawn.
         * Absent for environment spawns or timer-based spawns.
         */
        std::optional<EmitterContext> emitterContext;

    };


}