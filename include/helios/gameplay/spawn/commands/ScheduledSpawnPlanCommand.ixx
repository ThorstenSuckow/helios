/**
 * @file ScheduledSpawnPlanCommand.ixx
 * @brief Command for executing a scheduled spawn plan.
 */
module;

#include <cassert>
#include <memory>

export module helios.gameplay.spawn.commands.ScheduledSpawnPlanCommand;

import helios.gameplay.spawn.scheduling.ScheduledSpawnPlan;


import helios.gameplay.spawn.types.SpawnContext;
import helios.gameplay.spawn.scheduling.SpawnPlan;
import helios.gameplay.spawn.types.SpawnProfileId;

import helios.runtime.world.types.GameObjectHandle;

using namespace helios::gameplay::spawn::types;
using namespace helios::gameplay::spawn::scheduling;
using namespace helios::runtime::world::types;
export namespace helios::gameplay::spawn::commands {

    /**
     * @brief Command payload for execution of a scheduled spawn plan.
     *
     * @details This command is produced by scheduler-driven systems and consumed
     * by `SpawnManager<THandle>` via command-handler dispatch.
     *
     * @tparam THandle Handle type carried by spawn context data.
     *
     * @see ScheduledSpawnPlan
     * @see SpawnManager
     */
    template<typename THandle>
    class ScheduledSpawnPlanCommand {

        /**
         * @brief Profile identifying the spawn configuration.
         */
        helios::gameplay::spawn::types::SpawnProfileId spawnProfileId_;

        /**
         * @brief The spawn plan containing rule ID and amount.
         */
        helios::gameplay::spawn::scheduling::SpawnPlan spawnPlan_;

        /**
         * @brief Context providing spawn-time information.
         */
        SpawnContext<THandle> spawnContext_;

    public:

        /**
         * @brief Constructs a command with the given spawn plan.
         *
         * @param spawnProfileId The profile ID for spawning.
         * @param spawnPlan The spawn plan containing rule ID and amount.
         * @param spawnContext The context for spawn operations.
         */
        explicit ScheduledSpawnPlanCommand(
            const SpawnProfileId spawnProfileId,
            const SpawnPlan spawnPlan,
            const SpawnContext<THandle>& spawnContext
        ) :
            spawnProfileId_(spawnProfileId),
            spawnPlan_(spawnPlan),
            spawnContext_(spawnContext)
        {}

        /**
         * @brief Retrieves the spawn profile identifier associated with the command.
         *
         * @return The spawn profile identifier encapsulated within the ScheduledSpawnPlanCommand.
         */
        [[nodiscard]] SpawnProfileId spawnProfileId() const noexcept {
            return spawnProfileId_;
        }

        /**
         * @brief Returns the spawn plan associated with this instance.
         *
         * @return A structured plan that can be executed by the spawn management system.
         */
        [[nodiscard]] SpawnPlan spawnPlan() const noexcept {
            return spawnPlan_;
        }

        /**
         * @brief Retrieves the spawn context stored in this command.
         *
         * @return Spawn context specialized for `GameObjectHandle`.
         */
        [[nodiscard]] SpawnContext<GameObjectHandle> spawnContext() const noexcept {
            return spawnContext_;
        }

    };


}