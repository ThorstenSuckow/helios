/**
 * @file ScheduledSpawnPlanCommand.ixx
 * @brief Command for executing a scheduled spawn plan.
 */
module;

#include <memory>

export module helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;

import helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;
import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;
import helios.engine.runtime.world.GameWorld;

export namespace helios::engine::runtime::spawn::commands {

    /**
     * @brief Command for executing a scheduled spawn plan.
     *
     * @details ScheduledSpawnPlanCommand wraps a ScheduledSpawnPlan and routes
     * it to the appropriate dispatcher for execution. The command itself does
     * not perform spawning directly; instead, it delegates to a dispatcher
     * which forwards the request to the SpawnManager.
     *
     * ## Execution Flow
     *
     * 1. GameObjectSpawnSystem creates command from ScheduledSpawnPlan
     * 2. Command is added to CommandBuffer
     * 3. CommandBuffer flushes and calls accept() on each command
     * 4. Dispatcher routes command to SpawnManager
     * 5. SpawnManager processes the spawn request
     *
     * @see ScheduledSpawnPlan
     * @see ScheduledSpawnPlanCommandDispatcher
     * @see SpawnManager
     */
    class ScheduledSpawnPlanCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        /**
         * @brief The scheduled spawn plan to execute.
         */
        const helios::engine::runtime::spawn::scheduling::ScheduledSpawnPlan scheduledSpawnPlan_;

    public:

        /**
         * @brief Constructs a command with the given spawn plan.
         *
         * @param scheduledSpawnPlan The spawn plan to execute.
         */
        explicit ScheduledSpawnPlanCommand(
            helios::engine::runtime::spawn::scheduling::ScheduledSpawnPlan scheduledSpawnPlan
        ) :
            scheduledSpawnPlan_(std::move(scheduledSpawnPlan))
        {}

        /**
         * @brief Direct execution is a no-op; dispatching is required.
         *
         * @details This command requires a dispatcher to route it to the
         * SpawnManager. Direct execution does nothing.
         *
         * @param gameWorld The game world (unused).
         */
        void execute(helios::engine::runtime::world::GameWorld& gameWorld) const noexcept override {
            // noop - requires dispatcher
        }

        /**
         * @brief Accepts a dispatcher for command routing.
         *
         * @details Invokes the dispatcher's dispatch method, allowing it to
         * handle this command type appropriately (typically routing to SpawnManager).
         *
         * @param gameWorld The game world.
         * @param dispatcher The dispatcher to route this command to.
         */
        void accept(
            helios::engine::runtime::world::GameWorld& gameWorld,
            helios::engine::runtime::messaging::command::WorldCommandDispatcher& dispatcher
        ) const noexcept override {
            dispatcher.dispatch(gameWorld, *this);
        }

        /**
         * @brief Returns the scheduled spawn plan.
         *
         * @return The ScheduledSpawnPlan wrapped by this command.
         */
        [[nodiscard]] helios::engine::runtime::spawn::scheduling::ScheduledSpawnPlan scheduledSpawnPlan() const noexcept {
            return scheduledSpawnPlan_;
        }

    };


}