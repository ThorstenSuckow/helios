/**
 * @file DespawnCommandDispatcher.ixx
 * @brief Dispatcher for routing DespawnCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.DespawnCommandDispatcher;

import helios.engine.runtime.spawn.commands.DespawnCommand;
import helios.engine.runtime.spawn.requests.DespawnRequest;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes DespawnCommands to the appropriate handler.
     *
     * @details DespawnCommandDispatcher is a typed command dispatcher that handles
     * DespawnCommand instances. When a DespawnCommand is dispatched, it creates a
     * DespawnRequest and submits it to the pool's registered SpawnRequestHandler.
     *
     * The dispatcher requires that the DespawnCommand has a valid SpawnProfileId.
     * Despawning without pool ownership is not yet supported.
     *
     * @see DespawnCommand
     * @see SpawnManager
     * @see SpawnRequestHandler
     */
    class DespawnCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::DespawnCommand> {

    protected:

        /**
         * @brief Dispatches a DespawnCommand by creating a DespawnRequest.
         *
         * @details Creates a DespawnRequest from the command and submits it to
         * the pool's registered SpawnRequestHandler for deferred processing.
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

            const auto guid = command.guid();
            const auto spawnProfileId = command.spawnProfileId();

            if (!spawnProfileId) {
                assert(false && "Missing support for despawns w/o pool ownership");
                return;
            }

            auto* spawnRequestHandler = gameWorld.spawnRequestHandler(spawnProfileId.value());

            if (spawnRequestHandler) {
                helios::engine::runtime::spawn::requests::DespawnRequest despawnRequest(
                    guid, spawnProfileId
                );

                spawnRequestHandler->submit(despawnRequest);
            }
        }

    public:

        /**
         * @brief Default constructor.
         */
        DespawnCommandDispatcher() = default;

    };


}