/**
 * @file SpawnRequest.ixx
 * @brief Data structure for spawn requests processed by SpawnManager.
 */
module;

export module helios.engine.runtime.spawn.requests.SpawnRequest;

import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::runtime::spawn::requests {

    /**
     * @brief Data structure representing a request to spawn GameObjects.
     *
     * @details SpawnRequest is a lightweight data transfer object used by the
     * SpawnCommandDispatcher to communicate spawn parameters to the SpawnManager.
     * It identifies the spawn profile and provides context for placement and
     * initialization.
     *
     * @see SpawnCommand
     * @see SpawnManager
     * @see SpawnContext
     */
    struct SpawnRequest {

        /**
         * @brief The spawn profile defining pool, placer, and initializer.
         */
        const helios::engine::core::data::SpawnProfileId spawnProfileId;

        /**
         * @brief Context for spawn placement and initialization.
         *
         * Contains optional EmitterContext for emitter-relative spawning.
         */
        const helios::engine::runtime::spawn::SpawnContext spawnContext;

    };

}