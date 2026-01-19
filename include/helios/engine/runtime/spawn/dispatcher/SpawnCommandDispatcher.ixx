/**
 * @file SpawnCommandDispatcher.ixx
 * @brief Dispatcher for routing SpawnCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.SpawnCommandDispatcher;

import helios.engine.runtime.spawn.commands.SpawnCommand;
import helios.engine.runtime.spawn.requests.SpawnRequest;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes SpawnCommands to the appropriate handler.
     *
     * @details SpawnCommandDispatcher is a typed command dispatcher that handles
     * SpawnCommand instances. When a SpawnCommand is dispatched, it creates a
     * SpawnRequest and submits it to the pool's registered SpawnRequestHandler.
     *
     * This enables the command-based spawn architecture where the GameObjectSpawnSystem
     * emits SpawnCommands that are later processed by the SpawnManager during the
     * manager flush phase.
     *
     * @see SpawnCommand
     * @see SpawnManager
     * @see SpawnRequestHandler
     */
    class SpawnCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::SpawnCommand> {

    protected:

        /**
         * @brief Dispatches a SpawnCommand by creating a SpawnRequest.
         *
         * @details Creates a SpawnRequest from the command and submits it to
         * the pool's registered SpawnRequestHandler for deferred processing.
         *
         * @param gameWorld The game world containing the pool.
         * @param command The SpawnCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::spawn::commands::SpawnCommand& command
        ) noexcept override {


            auto spawnProfileId = command.spawnProfileId();

            auto* spawnRequestHandler = gameWorld.spawnRequestHandler(spawnProfileId);

            if (spawnRequestHandler) {
                helios::engine::runtime::spawn::requests::SpawnRequest spawnRequest(
                    spawnProfileId, command.spawnContext()
                );

                spawnRequestHandler->submit(spawnRequest);
            }
        };


    public:

         SpawnCommandDispatcher() = default;




    };


}