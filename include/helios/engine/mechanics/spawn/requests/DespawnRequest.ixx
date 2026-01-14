/**
 * @file DespawnRequest.ixx
 * @brief Data structure for despawn requests processed by SpawnManager.
 */
module;

#include <cinttypes>
#include <optional>

export module helios.engine.mechanics.spawn.requests.DespawnRequest;

import helios.math;

import helios.util.Guid;

import helios.engine.core.data.GameObjectPoolId;

export namespace helios::engine::mechanics::spawn::requests {

    /**
     * @brief Data structure representing a request to despawn a GameObject.
     *
     * @details DespawnRequest is a lightweight data transfer object used by the
     * DespawnCommandDispatcher to communicate despawn parameters to the SpawnManager.
     * It identifies the target entity by GUID and optionally specifies the pool
     * to return it to.
     *
     * @see DespawnCommand
     * @see SpawnManager
     */
    struct DespawnRequest {

        /**
         * @brief The GUID of the entity to despawn.
         */
        const helios::util::Guid entityId;

        /**
         * @brief Optional pool ID to return the entity to.
         *
         * @details If specified, the entity will be returned to this pool.
         * If nullopt, the SpawnManager must determine the correct pool or
         * handle the despawn without pool return.
         */
        const std::optional<helios::engine::core::data::GameObjectPoolId> gameObjectPoolId;
    };

}