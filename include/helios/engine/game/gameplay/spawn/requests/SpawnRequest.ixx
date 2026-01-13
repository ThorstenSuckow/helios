/**
 * @file SpawnRequest.ixx
 * @brief Data structure for spawn requests processed by SpawnManager.
 */
module;

#include <cinttypes>

export module helios.engine.game.gameplay.spawn.requests.SpawnRequest;

import helios.math;
import helios.engine.core.data.GameObjectPoolId;

export namespace helios::engine::game::gameplay::spawn::requests {

    /**
     * @brief Data structure representing a request to spawn GameObjects.
     *
     * @details SpawnRequest is a lightweight data transfer object used by the
     * SpawnCommandDispatcher to communicate spawn parameters to the SpawnManager.
     * It contains the target pool and the number of objects to spawn.
     *
     * @see SpawnCommand
     * @see SpawnManager
     */
    struct SpawnRequest {

        /**
         * @brief The ID of the pool from which to spawn objects.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId;

        /**
         * @brief The number of objects to spawn.
         */
        const size_t spawnBudget;
    };

}