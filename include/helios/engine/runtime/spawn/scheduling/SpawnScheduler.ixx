/**
 * @file SpawnScheduler.ixx
 * @brief Scheduler that evaluates spawn rules and produces spawn plans.
 */
module;

#include <span>
#include <vector>
#include <algorithm>

export module helios.engine.runtime.spawn.scheduling.SpawnScheduler;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.spawn.SpawnContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;
import helios.engine.core.data.SpawnProfileId;
import helios.engine.core.data.SpawnRuleId;
import helios.engine.runtime.spawn.policy.SpawnRule;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.pooling.GameObjectPoolManager;

export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief Abstract base class for spawn schedulers.
     *
     * @details SpawnScheduler defines the interface for evaluating spawn rules
     * and producing scheduled spawn plans. Concrete implementations determine
     * the evaluation strategy (all rules vs. cyclic, priority-based, etc.).
     *
     * ## Responsibilities
     *
     * - Maintain a buffer of scheduled spawn plans
     * - Provide `evaluate()` for rule processing
     * - Provide `drainScheduledPlans()` for retrieving pending spawns
     * - Provide `commit()` for post-spawn state updates
     *
     * ## Implementations
     *
     * | Class | Strategy |
     * |-------|----------|
     * | `DefaultSpawnScheduler` | Evaluates all rules each frame |
     * | `CyclicSpawnScheduler` | Round-robin evaluation, advances on successful spawn |
     *
     * @see DefaultSpawnScheduler
     * @see CyclicSpawnScheduler
     * @see ScheduledSpawnPlan
     */
    class SpawnScheduler {

    protected:

        /**
         * @brief Buffer for scheduled spawn plans awaiting processing.
         */
        std::vector<ScheduledSpawnPlan> scheduledSpawnPlans_;

    public:

        /**
         * @brief Virtual destructor for proper cleanup.
         */
        virtual ~SpawnScheduler() = default;

        /**
         * @brief Default constructor.
         */
        SpawnScheduler() = default;

        /**
         * @brief Evaluates spawn rules and schedules spawn plans.
         *
         * @details Processes registered spawn rules according to the scheduler's
         * strategy. Produces ScheduledSpawnPlan instances for rules whose
         * conditions are met.
         *
         * @param updateContext Current frame context with delta time and world.
         * @param spawnContext Optional spawn context for the operation.
         */
        virtual void evaluate(
            const helios::engine::runtime::world::UpdateContext& updateContext,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext = {})  noexcept = 0;


        /**
         * @brief Returns a read-only view of scheduled spawn plans.
         *
         * @return Span of currently scheduled plans.
         */
        [[nodiscard]] std::span<const ScheduledSpawnPlan> scheduledPlans() {
            return scheduledSpawnPlans_;
        }

        /**
         * @brief Drains and returns all scheduled spawn plans.
         *
         * @details Moves all pending plans out of the scheduler and returns them.
         * The internal buffer is left empty.
         *
         * @return Vector of scheduled spawn plans.
         */
        [[nodiscard]] std::vector<ScheduledSpawnPlan> drainScheduledPlans() {
            std::vector<ScheduledSpawnPlan> plans{};
            plans.swap(scheduledSpawnPlans_);
            return plans;
        }


        /**
         * @brief Commits a completed spawn operation to update rule state.
         *
         * @details Called when a spawn request has been executed to update
         * the associated rule's state (e.g., reset timers, update counts).
         *
         * @param spawnRuleId The rule that triggered the spawn.
         * @param spawnCount The number of entities actually spawned.
         */
        virtual void commit(const helios::engine::core::data::SpawnRuleId spawnRuleId, const size_t spawnCount) noexcept = 0;

        /**
         * @brief Resets all rule states to their initial values.
         *
         * @details Called during level transitions or game restarts.
         */
        virtual void reset() noexcept = 0;
    };

}
