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
import helios.engine.runtime.spawn.events.SpawnPlanCommandExecutedEvent;

export namespace helios::engine::mechanics::spawn::systems {

    /**
     * @brief System that evaluates spawn conditions and enqueues spawn commands.
     *
     * @details GameObjectSpawnSystem bridges the spawn scheduling logic with the
     * command-based spawning pipeline. It manages multiple SpawnScheduler instances
     * and performs the following each frame:
     *
     * 1. **Read Frame Events:** Consumes `SpawnPlanCommandExecutedEvent` from the
     *    previous frame to commit completed spawn operations to all schedulers.
     *
     * 2. **Evaluate Conditions:** Calls `SpawnScheduler::evaluate()` on each
     *    scheduler to check all registered spawn rules against their conditions.
     *
     * 3. **Drain Scheduled Plans:** Retrieves all plans that are ready to execute
     *    and enqueues them as `ScheduledSpawnPlanCommand` into the CommandBuffer.
     *
     * The actual spawning is performed by the command dispatcher/manager layer,
     * keeping this system focused on scheduling logic only.
     *
     * Example setup:
     * ```cpp
     * std::vector<std::unique_ptr<SpawnScheduler>> schedulers;
     *
     * auto enemyScheduler = std::make_unique<SpawnScheduler>();
     * enemyScheduler->addRule(enemyProfileId, std::move(enemyRule));
     * schedulers.push_back(std::move(enemyScheduler));
     *
     * auto powerupScheduler = std::make_unique<SpawnScheduler>();
     * powerupScheduler->addRule(powerupProfileId, std::move(powerupRule));
     * schedulers.push_back(std::move(powerupScheduler));
     *
     * auto spawnSystem = std::make_unique<GameObjectSpawnSystem>(schedulers);
     * mainPhase.addPass().add(std::move(spawnSystem));
     * ```
     *
     * @note This system reads frame-level events via `readFrame()`, meaning it
     *       processes spawn confirmations from the previous frame.
     *
     * @see SpawnScheduler
     * @see ScheduledSpawnPlanCommand
     * @see SpawnPlanCommandExecutedEvent
     */
    class GameObjectSpawnSystem : public helios::engine::ecs::System {

        /**
         * @brief Collection of schedulers that manage spawn rules and conditions.
         *
         * @details Each scheduler owns and evaluates its registered spawn rules
         * independently. When conditions are met, the scheduler produces
         * ScheduledSpawnPlan instances for execution. Multiple schedulers allow
         * grouping spawn rules by category (e.g., enemies, powerups, projectiles).
         */
        std::vector<std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler>> spawnSchedulers_;

    public:

        /**
         * @brief Constructs a GameObjectSpawnSystem with the given schedulers.
         *
         * @param spawnSchedulers Vector of schedulers managing spawn rules.
         *        Ownership of all schedulers is transferred to this system.
         */
        explicit GameObjectSpawnSystem(
             std::vector<std::unique_ptr<helios::engine::runtime::spawn::scheduling::SpawnScheduler>>& spawnSchedulers
        ) :
            spawnSchedulers_(std::move(spawnSchedulers)) {}

        /**
         * @brief Processes spawn scheduling and enqueues spawn commands.
         *
         * @details Iterates through all schedulers and performs these steps for each:
         * 1. Reads `SpawnPlanCommandExecutedEvent` from frame event bus
         * 2. Commits completed spawns to the scheduler (for tracking/cooldowns)
         * 3. Evaluates all spawn rules against current conditions
         * 4. Drains ready spawn plans and enqueues them as commands
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            const auto& events = updateContext.readFrame<
                helios::engine::runtime::spawn::events::SpawnPlanCommandExecutedEvent
            >();

            for (const auto& spawnScheduler : spawnSchedulers_) {

                /**
                 * @todo this should be processed before iterating over all schedulers: A scheduler owns the rule,
                 * hence a rule that triggered the event's can be associated with the owning Scheduler, making it
                 * unneccesary to iterate over already processed events
                 */
                for (const auto& event : events) {
                    spawnScheduler->commit(event.spawnRuleId, event.spawnCount);
                }

                spawnScheduler->evaluate(updateContext);

                auto scheduledPlans = spawnScheduler->drainScheduledPlans();

                for (auto& plan : scheduledPlans) {
                    updateContext.commandBuffer().add<
                        helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand
                    >(
                        plan.spawnProfileId, plan.spawnPlan, plan.spawnContext
                    );
                }
            }

        }

    };

}
