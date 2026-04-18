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


import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;
import helios.runtime.spawn.commands.ScheduledSpawnPlanCommand;
import helios.runtime.spawn.SpawnManager;
import helios.runtime.spawn.scheduling.SpawnScheduler;
import helios.runtime.spawn.events.SpawnPlanCommandExecutedEvent;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;
import helios.engine.common.tags.SystemRole;


using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;
export namespace helios::engine::mechanics::spawn::systems {

    template<typename THandle, typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class GameObjectSpawnSystem {


        helios::runtime::spawn::SpawnManager<THandle>& spawnManager_;

       GameWorld* gameWorld_ = nullptr;

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        explicit GameObjectSpawnSystem(helios::runtime::spawn::SpawnManager<THandle>& spawnManager) noexcept
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
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            const auto& events = updateContext.readFrame<
                helios::runtime::spawn::events::SpawnPlanCommandExecutedEvent
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
                    updateContext.queueCommand<TCommandBuffer,
                        helios::runtime::spawn::commands::ScheduledSpawnPlanCommand<THandle>
                    >(
                        plan.spawnProfileId, plan.spawnPlan, plan.spawnContext
                    );
                }
            }

        }

    };

}
