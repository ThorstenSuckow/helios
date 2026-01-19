/**
 * @file ScheduledSpawnPlanCommandDispatcher.ixx
 * @brief Dispatcher for routing ScheduledSpawnPlanCommands to the SpawnManager.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.dispatcher.ScheduledSpawnPlanCommandDispatcher;

import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.spawn.requests.ScheduledSpawnPlanRequest;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::spawn::dispatcher {

    /**
     * @brief Dispatcher that routes ScheduledSpawnPlanCommands to the appropriate handler.
     *
     * @details ScheduledSpawnPlanCommandDispatcher is a typed command dispatcher that
     * handles ScheduledSpawnPlanCommand instances. When a command is dispatched, it
     * creates a ScheduledSpawnPlanRequest and submits it to the pool's registered
     * SpawnRequestHandler.
     *
     * This dispatcher bridges the SpawnScheduler output (ScheduledSpawnPlan) with
     * the SpawnManager's request processing pipeline.
     *
     * @see ScheduledSpawnPlanCommand
     * @see SpawnScheduler
     * @see SpawnManager
     */
    class ScheduledSpawnPlanCommandDispatcher final : public helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<
        helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand> {

    protected:

        /**
         * @brief Dispatches a ScheduledSpawnPlanCommand by creating a request.
         *
         * @details Creates a ScheduledSpawnPlanRequest from the command and submits
         * it to the profile's registered SpawnRequestHandler for deferred processing.
         *
         * @param gameWorld The game world containing the pool.
         * @param command The ScheduledSpawnPlanCommand to dispatch.
         */
        void dispatchTyped(
            helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand& command
        ) noexcept override {

            auto scheduledSpawnPlan = command.scheduledSpawnPlan();
            auto* spawnRequestHandler = gameWorld.spawnRequestHandler(
                scheduledSpawnPlan.spawnProfileId
            );

            if (spawnRequestHandler) {
                helios::engine::runtime::spawn::requests::ScheduledSpawnPlanRequest scheduledSpawnPlanRequest{
                    scheduledSpawnPlan, {}
                };

                spawnRequestHandler->submit(scheduledSpawnPlanRequest);
            }
        }

    public:

        /**
         * @brief Default constructor.
         */
        ScheduledSpawnPlanCommandDispatcher() = default;

    };


}