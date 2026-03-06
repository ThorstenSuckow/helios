/**
 * @file GameObjectSpawnSystem.ixx
 * @brief System for processing spawn schedules and enqueuing spawn commands.
 */
module;

#include <cassert>
#include <memory>
#include <vector>

export module helios.engine.mechanics.spawn.systems.GameObjectSpawnSystem;



import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.engine.runtime.spawn.SpawnManager;
import helios.engine.runtime.spawn.scheduling.SpawnScheduler;
import helios.engine.runtime.spawn.events.SpawnPlanCommandExecutedEvent;

using namespace helios::engine::runtime::world;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::mechanics::spawn::systems {


    class GameObjectSpawnSystem {


        helios::engine::runtime::spawn::SpawnManager& spawnManager_;

       GameWorld* gameWorld_;

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        explicit GameObjectSpawnSystem(helios::engine::runtime::spawn::SpawnManager& spawnManager) noexcept
        : spawnManager_{spawnManager} {}


        void init(GameWorld& gameWorld) noexcept {
            gameWorld_ = &gameWorld;
        }

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
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

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

                spawnScheduler->evaluate(*gameWorld_, updateContext);

                auto scheduledPlans = spawnScheduler->drainScheduledPlans();

                for (auto& plan : scheduledPlans) {
                    updateContext.queueCommand<
                        helios::engine::runtime::spawn::commands::ScheduledSpawnPlanCommand
                    >(
                        plan.spawnProfileId, plan.spawnPlan, plan.spawnContext
                    );
                }
            }

        }

    };

}
