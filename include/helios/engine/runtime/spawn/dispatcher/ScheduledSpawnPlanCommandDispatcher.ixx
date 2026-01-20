/**
 * @file ScheduledSpawnPlanCommandDispatcher.ixx
 * @brief Dispatcher for routing ScheduledSpawnPlanCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.ScheduledSpawnPlanCommandDispatcher;

import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes ScheduledSpawnPlanCommands to the appropriate handler.
     *
     * @details ScheduledSpawnPlanCommandDispatcher is a typed command dispatcher that
     * handles ScheduledSpawnPlanCommand instances. When a command is dispatched, it
     * submits the command directly to the pool's registered SpawnCommandHandler.
     *
     * This dispatcher bridges the SpawnScheduler output (ScheduledSpawnPlan) with
     * the SpawnManager's command processing pipeline.
     *
     * @see ScheduledSpawnPlanCommand
     * @see SpawnScheduler
     * @see SpawnManager
     */
    class ScheduledSpawnPlanCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand> {

    protected:

        /**
         * @brief Dispatches a ScheduledSpawnPlanCommand to the registered handler.
         *
         * @details Looks up the SpawnCommandHandler for the command's profile ID
         * and submits the command directly for deferred processing.
         *
         * @param gameWorld The game world containing the pool.
         * @param command The ScheduledSpawnPlanCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand& command
        ) noexcept override {

            if (auto* spawnCommandHandler = gameWorld.spawnCommandHandler(
                command.spawnProfileId()
            )) {
                spawnCommandHandler->submit(command);
            }
        }

    public:

        /**
         * @brief Default constructor.
         */
        ScheduledSpawnPlanCommandDispatcher() = default;

    };


}