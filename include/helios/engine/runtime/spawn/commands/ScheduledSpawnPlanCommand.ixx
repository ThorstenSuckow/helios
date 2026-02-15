/**
 * @file ScheduledSpawnPlanCommand.ixx
 * @brief Command for executing a scheduled spawn plan.
 */
module;

#include <cassert>
#include <memory>

export module helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;

import helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;
import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;

import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.core.data.SpawnProfileId;

export namespace helios::engine::runtime::spawn::commands {

    /**
     * @brief Command for executing a scheduled spawn plan.
     *
     * @details ScheduledSpawnPlanCommand wraps a ScheduledSpawnPlan and routes
     * it to the appropriate dispatcher for execution. The command itself does
     * not perform spawning directly; instead, it delegates to a dispatcher
     * which forwards the command to the SpawnManager.
     *
     * ## Execution Flow
     *
     * 1. GameObjectSpawnSystem creates command from ScheduledSpawnPlan
     * 2. Command is added to CommandBuffer
     * 3. CommandBuffer flushes and calls accept() on each command
     * 4. Dispatcher routes command to SpawnManager
     * 5. SpawnManager processes the spawn command
     *
     * @see ScheduledSpawnPlan
     * @see ScheduledSpawnPlanCommandDispatcher
     * @see SpawnManager
     */
    class ScheduledSpawnPlanCommand : public helios::engine::runtime::messaging::command::WorldCommand {

        helios::engine::core::data::SpawnProfileId spawnProfileId_;

        helios::engine::runtime::spawn::scheduling::SpawnPlan spawnPlan_;

        helios::engine::runtime::spawn::SpawnContext spawnContext_;

    public:

        /**
         * @brief Constructs a command with the given spawn plan.
         *
         * @param spawnProfileId The profile ID for spawning.
         * @param spawnPlan The spawn plan containing rule ID and amount.
         * @param spawnContext The context for spawn operations.
         */
        explicit ScheduledSpawnPlanCommand(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            const helios::engine::runtime::spawn::scheduling::SpawnPlan spawnPlan,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext
        ) :
            spawnProfileId_(spawnProfileId),
            spawnPlan_(spawnPlan),
            spawnContext_(spawnContext)
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
            assert(false && "Unexpected execute() on ScheduledSpawnPlanCommand");
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
         * @brief Retrieves the spawn profile identifier associated with the command.
         *
         * @return The spawn profile identifier encapsulated within the ScheduledSpawnPlanCommand.
         */
        [[nodiscard]] helios::engine::core::data::SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

        /**
         * @brief Returns the spawn plan associated with this instance.
         *
         * @return A structured plan that can be executed by the spawn management system.
         */
        [[nodiscard]] helios::engine::runtime::spawn::scheduling::SpawnPlan spawnPlan() const noexcept {
            return spawnPlan_;
        }


        /**
         * @brief Retrieves the spawn context associated with this instance.
         *
         * @return The spawn context.
         */
        [[nodiscard]] helios::engine::runtime::spawn::SpawnContext spawnContext() const noexcept {
            return spawnContext_;
        }

    };


}