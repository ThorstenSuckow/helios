/**
 * @file SpawnScheduler.ixx
 * @brief Scheduler that evaluates spawn rules and produces spawn plans.
 */
module;

#include <cassert>
#include <memory>
#include <ranges>
#include <span>
#include <unordered_map>
#include <vector>

export module helios.engine.runtime.spawn.scheduling.SpawnScheduler;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.spawn.SpawnManager;
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
     * @brief Scheduler that evaluates spawn rules and produces spawn plans.
     *
     * @details SpawnScheduler is the central coordinator for rule-based spawning.
     * It maintains a collection of spawn rules, evaluates them each frame, and
     * produces ScheduledSpawnPlan instances for rules whose conditions are met.
     *
     * ## Workflow
     *
     * 1. **Registration:** Rules are added via `addRule()` with their profile ID
     * 2. **Evaluation:** `evaluate()` is called each frame by GameObjectSpawnSystem
     * 3. **Scheduling:** Rules that pass conditions produce ScheduledSpawnPlans
     * 4. **Draining:** `drainScheduledPlans()` returns and clears pending plans
     * 5. **Commit:** `commit()` is called when spawns complete to update state
     *
     * Example:
     * ```cpp
     * SpawnScheduler scheduler;
     * scheduler.addRule(enemyProfileId, std::make_unique<TimerSpawnRule>(
     *     ruleId, 2.0f, 3  // Every 2s, spawn 3
     * ));
     *
     * // In game loop
     * scheduler.evaluate(updateContext);
     * auto plans = scheduler.drainScheduledPlans();
     * ```
     *
     * @see SpawnRule
     * @see ScheduledSpawnPlan
     * @see GameObjectSpawnSystem
     */
    class SpawnScheduler {

        /**
         * @brief Map from spawn profile IDs to their spawn rules.
         */
        std::unordered_map<
            helios::engine::core::data::SpawnProfileId,
            std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule>
        > spawnRules_;

        /**
         * @brief Map from spawn rule IDs to their runtime state.
         */
        std::unordered_map<
            helios::engine::core::data::SpawnRuleId,
            helios::engine::runtime::spawn::policy::SpawnRuleState
        > spawnRuleStates_;

        /**
         * @brief Buffer for scheduled spawn plans awaiting processing.
         */
        std::vector<ScheduledSpawnPlan> scheduledSpawnPlans_;

    public:

        /**
         * @brief Constructs a SpawnScheduler with optional initial capacity.
         *
         * @param initialSpanPlanSize Initial capacity for the spawn plan buffer.
         */
        SpawnScheduler(const size_t initialSpanPlanSize = 20) {
            scheduledSpawnPlans_.reserve(initialSpanPlanSize);
        }

        /**
         * @brief Evaluates all spawn rules against current conditions.
         *
         * @details Iterates through all registered rules, updates their state,
         * and creates ScheduledSpawnPlan entries for rules that should spawn.
         *
         * @param updateContext The current frame's update context.
         */
        void evaluate(
            const helios::engine::runtime::world::UpdateContext& updateContext,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext = {}) {

            const auto* poolManager  = updateContext.gameWorld().getManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
            const auto* spawnManager = updateContext.gameWorld().getManager<helios::engine::runtime::spawn::SpawnManager>();

            scheduledSpawnPlans_.clear();

            for (auto& [spawnProfileId, rule] : spawnRules_) {

                const auto* spawnProfile = spawnManager->spawnProfile(spawnProfileId);
                if (!spawnProfile) {
                    /**
                     * @todo remove stale entry
                     */
                    continue;
                }

                const auto& [gameObjectPoolId, _, __] = *spawnProfile;

                auto& spawnRuleState = spawnRuleStates_[rule->spawnRuleId()];

                const auto poolSnapshot = poolManager->poolSnapshot(gameObjectPoolId);

                // tick the rule state
                spawnRuleState.update(updateContext.deltaTime());

                auto spawnPlan = rule->evaluate(
                    gameObjectPoolId, poolSnapshot,
                    spawnRuleState,
                    updateContext
                );

                if (spawnPlan.amount == 0) {
                    continue;
                }

                scheduledSpawnPlans_.push_back({
                    spawnProfileId,
                    std::move(spawnPlan),
                    spawnContext
                });

            }

        }

        /**
         * @brief Returns a view of scheduled spawn plans.
         *
         * @return Span of scheduled spawn plans.
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
         * @brief Adds a spawn rule for a profile.
         *
         * @param spawnProfileId The profile ID to associate with the rule.
         * @param spawnRule The spawn rule to add. Ownership transferred.
         *
         * @return Reference to this scheduler for chaining.
         *
         * @pre No rule is already registered for this profile ID.
         */
        SpawnScheduler& addRule(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule> spawnRule
        ) {
            assert(!spawnRules_.contains(spawnProfileId) && "Duplicate SpawnProfile entry");
            assert(!spawnRuleStates_.contains(spawnRule->spawnRuleId()) && "Duplicate SpawnRuleId entry");

            spawnRuleStates_.try_emplace(spawnRule->spawnRuleId());
            spawnRules_.try_emplace(spawnProfileId, std::move(spawnRule));


            return *this;
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
        void commit(const helios::engine::core::data::SpawnRuleId spawnRuleId, const size_t spawnCount) {

            for (auto& spawnRule : spawnRules_ | std::views::values) {

                if (spawnRule->spawnRuleId() == spawnRuleId) {

                    auto it = spawnRuleStates_.find(spawnRuleId);
                    assert(it != spawnRuleStates_.end() && "Unexpected missing spawnRuleState");

                    spawnRule->commit(it->second, spawnCount);
                }
            }
        }

    };

}
