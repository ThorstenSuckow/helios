/**
 * @file GameObjectSpawnSystem.ixx
 * @brief System for processing spawn schedules and enqueuing spawn commands.
 */
module;

#include <cassert>
#include <memory>
#include <vector>

export module helios.engine.mechanics.spawn.systems.GameObjectSpawnSystem;

import helios.engine.ecs.System;
import helios.engine.runtime.messaging.command.CommandBuffer;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.spawn.scheduling.SpawnScheduler;
import helios.engine.runtime.spawn.events.SpawnPlanRequestExecutedEvent;

export namespace helios::engine::mechanics::spawn::systems {

    /**
     * @brief System that evaluates spawn conditions and enqueues spawn commands.
     *
     * @details GameObjectSpawnSystem bridges the spawn scheduling logic with the
     * command-based spawning pipeline. It performs the following each frame:
     *
     * 1. **Read Frame Events:** Consumes `SpawnPlanRequestExecutedEvent` from the
     *    previous frame to commit completed spawn operations to the scheduler.
     *
     * 2. **Evaluate Conditions:** Calls `SpawnScheduler::evaluate()` to check all
     *    registered spawn rules against their conditions (timer, trigger, etc.).
     *
     * 3. **Drain Scheduled Plans:** Retrieves all plans that are ready to execute
     *    and enqueues them as `ScheduledSpawnPlanCommand` into the CommandBuffer.
     *
     * The actual spawning is performed by the command dispatcher/manager layer,
     * keeping this system focused on scheduling logic only.
     *
     * Example setup:
     * ```cpp
     * auto scheduler = std::make_unique<SpawnScheduler>();
     * scheduler->addRule(enemyRuleId, enemyCondition, enemyProfileId);
     *
     * auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(
     *     std::move(scheduler)
     * );
     * mainPhase.addPass().add(std::move(spawnSystem));
     * ```
     *
     * @note This system reads frame-level events via `readFrame()`, meaning it
     *       processes spawn confirmations from the previous frame.
     *
     * @see SpawnScheduler
     * @see ScheduledSpawnPlanCommand
     * @see SpawnPlanRequestExecutedEvent
     */
    class GameObjectSpawnSystem : public helios::engine::ecs::System {

        /**
         * @brief The scheduler that manages spawn rules and conditions.
         *
         * Owns and evaluates all registered spawn rules. When conditions are met,
         * the scheduler produces ScheduledSpawnPlan instances for execution.
         */
        std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler> spawnScheduler_;

    public:

        /**
         * @brief Constructs a GameObjectSpawnSystem with the given scheduler.
         *
         * @param spawnScheduler The scheduler managing spawn rules. Ownership transferred.
         *
         * @pre spawnScheduler must not be null.
         */
        explicit GameObjectSpawnSystem(
            std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler> spawnScheduler
        ) :
            spawnScheduler_(std::move(spawnScheduler))
        {
            assert(spawnScheduler_ != nullptr && "Unexpected nullptr for SpawnScheduler");
        }

        /**
         * @brief Processes spawn scheduling and enqueues spawn commands.
         *
         * @details Performs the following steps:
         * 1. Reads `SpawnPlanRequestExecutedEvent` from frame event bus
         * 2. Commits completed spawns to the scheduler (for tracking/cooldowns)
         * 3. Evaluates all spawn rules against current conditions
         * 4. Drains ready spawn plans and enqueues them as commands
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            const auto& events = updateContext.readFrame<
                helios::engine::runtime::spawn::events::SpawnPlanRequestExecutedEvent
            >();

            for (const auto& event : events) {
                spawnScheduler_->commit(event.spawnRuleId, event.spawnCount);
            }

            spawnScheduler_->evaluate(updateContext);

            auto scheduledPlans = spawnScheduler_->drainScheduledPlans();

            for (auto& plan : scheduledPlans) {
                updateContext.commandBuffer().add<
                    helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand
                >(std::move(plan));
            }

        }

    };

}
