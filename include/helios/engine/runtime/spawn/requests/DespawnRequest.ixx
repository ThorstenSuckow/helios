/**
 * @file DespawnRequest.ixx
 * @brief Data structure for despawn requests processed by SpawnManager.
 */
module;

#include <optional>

export module helios.engine.runtime.spawn.requests.DespawnRequest;

import helios.util.Guid;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::runtime::spawn::requests {

    /**
     * @brief Data structure representing a request to despawn a GameObject.
     *
     * @details DespawnRequest is a lightweight data transfer object used by the
     * DespawnCommandDispatcher to communicate despawn parameters to the SpawnManager.
     * It identifies the target entity by GUID and optionally specifies the pool
     * to return it to.
     *
     * @see DespawnCommand
     * @see DespawnCommandDispatcher
     * @see SpawnManager
     */
    struct DespawnRequest {

        /**
         * @brief The GUID of the entity to despawn.
         */
        const helios::util::Guid entityId;

        /**
         * @brief Optional spawn profile ID for returning entity to correct pool.
         *
         * If not set, the SpawnManager must determine the pool from the entity's
         * SpawnedByProfileComponent or handle the despawn without pool return.
         */
        const std::optional<helios::engine::core::data::SpawnProfileId> spawnProfileId;

    };

}