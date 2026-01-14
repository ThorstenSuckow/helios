/**
 * @file SpawnCommandDispatcher.ixx
 * @brief Dispatcher for routing SpawnCommands to the SpawnManager.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.mechanics.spawn.dispatcher.SpawnCommandDispatcher;


import helios.engine.mechanics.spawn.manager.SpawnManager;
import helios.engine.mechanics.spawn.commands.SpawnCommand;

import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.mechanics.combat.components.Aim2DComponent;

import helios.engine.mechanics.spawn.requests.SpawnRequest;
          

import helios.engine.runtime.messaging.command.WorldCommandDispatcher;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.ecs.GameObject;

import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::mechanics::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes SpawnCommands to the appropriate handler.
     *
     * @details SpawnCommandDispatcher is a typed command dispatcher that handles
     * SpawnCommand instances. When a SpawnCommand is dispatched, it creates a
     * SpawnRequest and submits it to the pool's registered PoolRequestHandler.
     *
     * This enables the command-based spawn architecture where the GameObjectSpawnSystem
     * emits SpawnCommands that are later processed by the SpawnManager during the
     * manager flush phase.
     *
     * @see SpawnCommand
     * @see SpawnManager
     * @see PoolRequestHandler
     */
    class SpawnCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::mechanics::spawn::commands::SpawnCommand> {

        /**
         * @brief Reference to the SpawnManager for handling spawn requests.
         */
        helios::engine::mechanics::spawn::manager::SpawnManager& spawnManager_;

    protected:

        /**
         * @brief Dispatches a SpawnCommand by creating a SpawnRequest.
         *
         * @details Creates a SpawnRequest from the command and submits it to
         * the pool's registered PoolRequestHandler for deferred processing.
         *
         * @param gameWorld The game world containing the pool.
         * @param command The SpawnCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::mechanics::spawn::commands::SpawnCommand& command
        ) noexcept override {


            auto* poolRequestHandler = gameWorld.poolRequestHandler(command.gameObjectPoolId());

            if (poolRequestHandler) {
                helios::engine::mechanics::spawn::requests::SpawnRequest spawnRequest{
                    command.gameObjectPoolId(),
                    command.spawnBudget()

                };

                poolRequestHandler->submit(spawnRequest);
            }

        };

    public:

        /**
         * @brief Constructs a SpawnCommandDispatcher.
         *
         * @param spawnManager Reference to the SpawnManager to route requests to.
         */
        explicit SpawnCommandDispatcher(
            helios::engine::mechanics::spawn::manager::SpawnManager& spawnManager
        ) :
        spawnManager_(spawnManager)
        {}




    };


}