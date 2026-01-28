/**
 * @file SpawnCommandDispatcher.ixx
 * @brief Dispatcher for routing SpawnCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.SpawnCommandDispatcher;

import helios.engine.runtime.spawn.commands.SpawnCommand;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;


export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes SpawnCommands to the appropriate handler.
     *
     * @details SpawnCommandDispatcher is a typed command dispatcher that handles
     * SpawnCommand instances. When a SpawnCommand is dispatched, it submits the
     * command directly to the pool's registered SpawnCommandHandler.
     *
     * This enables the command-based spawn architecture where the GameObjectSpawnSystem
     * emits SpawnCommands that are later processed by the SpawnManager during the
     * manager flush phase.
     *
     * @see SpawnCommand
     * @see SpawnManager
     * @see SpawnCommandHandler
     */
    class SpawnCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::SpawnCommand> {

    protected:

        /**
         * @brief Dispatches a SpawnCommand to the registered handler.
         *
         * @details Looks up the SpawnCommandHandler for the command's profile ID
         * and submits the command directly for deferred processing.
         *
         * @param gameWorld The game world containing the pool.
         * @param command The SpawnCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::spawn::commands::SpawnCommand& command
        ) noexcept override {

            auto spawnProfileId = command.spawnProfileId();

            if (auto* spawnCommandHandler = gameWorld.spawnCommandHandler(spawnProfileId)) {
                spawnCommandHandler->submit(command);
            }
        };


    public:

         SpawnCommandDispatcher() = default;




    };


}