/**
 * @file DespawnCommandDispatcher.ixx
 * @brief Dispatcher for routing DespawnCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.DespawnCommandDispatcher;

import helios.engine.runtime.spawn.commands.DespawnCommand;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes DespawnCommands to the appropriate handler.
     *
     * @details DespawnCommandDispatcher is a typed command dispatcher that handles
     * DespawnCommand instances. When a DespawnCommand is dispatched, it submits
     * the command directly to the pool's registered SpawnCommandHandler.
     *
     * The dispatcher requires that the DespawnCommand has a valid SpawnProfileId.
     * Despawning without pool ownership is not yet supported.
     *
     * @see DespawnCommand
     * @see SpawnManager
     * @see SpawnCommandHandler
     */
    class DespawnCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::DespawnCommand> {

    protected:

        /**
         * @brief Dispatches a DespawnCommand to the registered handler.
         *
         * @details Looks up the SpawnCommandHandler for the command's profile ID
         * and submits the command directly for deferred processing.
         *
         * @param gameWorld The game world containing the entity.
         * @param command The DespawnCommand to dispatch.
         *
         * @pre command.spawnProfileId() must have a value.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::spawn::commands::DespawnCommand& command
        ) noexcept override {

            if (auto* spawnCommandHandler = gameWorld.spawnCommandHandler(command.spawnProfileId())) {
                spawnCommandHandler->submit(command);
            }
        }

    public:

        /**
         * @brief Default constructor.
         */
        DespawnCommandDispatcher() = default;

    };


}