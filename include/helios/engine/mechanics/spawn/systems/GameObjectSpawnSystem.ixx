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
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.spawn.SpawnManager;
import helios.engine.runtime.spawn.scheduling.SpawnScheduler;
import helios.engine.runtime.spawn.events.SpawnPlanCommandExecutedEvent;

export namespace helios::engine::mechanics::spawn::systems {


    class GameObjectSpawnSystem : public helios::engine::ecs::System {


        helios::engine::runtime::spawn::SpawnManager& spawnManager_;

    public:


        explicit GameObjectSpawnSystem(helios::engine::runtime::spawn::SpawnManager& spawnManager) noexcept
        : spawnManager_{spawnManager} {}


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

            for (const auto& spawnScheduler : spawnManager_.spawnSchedulers()) {

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
