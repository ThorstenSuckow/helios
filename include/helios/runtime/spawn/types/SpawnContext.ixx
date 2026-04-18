/**
 * @file SpawnContext.ixx
 * @brief Context data passed to spawn behaviors during entity initialization.
 */
module;

#include <optional>

export module helios.runtime.spawn.types.SpawnContext;

import helios.runtime.spawn.types.EmitterContext;

export namespace helios::runtime::spawn::types {

    /**
     * @brief Context data passed to spawn behaviors during entity initialization.
     *
     * @details `SpawnContext<THandle>` aggregates optional runtime data used by
     * `SpawnPlacer<THandle>` and `SpawnInitializer<THandle>`.
     *
     * @tparam THandle Handle type used by emitter/source references.
     *
     * @see EmitterContext
     */
    template<typename THandle>
    struct SpawnContext {

        /**
         * @brief Optional context from the entity that triggered the spawn.
         *
         * Present when an entity (e.g., spaceship) triggered the spawn.
         * Absent for environment spawns or timer-based spawns.
         */
        std::optional<EmitterContext<THandle>> emitterContext;

    };


}