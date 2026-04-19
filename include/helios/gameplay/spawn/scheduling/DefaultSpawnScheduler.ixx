/**
 * @file DefaultSpawnScheduler.ixx
 * @brief Default scheduler that evaluates all spawn rules each frame.
 */
module;

#include <cassert>
#include <memory>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cstddef>

export module helios.gameplay.spawn.scheduling.DefaultSpawnScheduler;

import helios.gameplay.spawn.scheduling.SpawnScheduler;
import helios.gameplay.spawn.scheduling.DefaultRuleProcessor;

import helios.runtime.world.UpdateContext;
import helios.gameplay.spawn.SpawnManager;
import helios.gameplay.spawn.types.SpawnContext;
import helios.runtime.world.GameWorld;
import helios.gameplay.spawn.scheduling.SpawnPlan;
import helios.gameplay.spawn.scheduling.ScheduledSpawnPlan;
import helios.gameplay.spawn.types;
import helios.gameplay.spawn.policy.SpawnRule;
import helios.gameplay.spawn.policy.SpawnRuleState;
import helios.runtime.pooling.EntityPoolManager;

using namespace helios::gameplay::spawn::types;
using namespace helios::runtime::world;
export namespace helios::gameplay::spawn::scheduling {


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
    template<typename THandle>
    class DefaultSpawnScheduler : public SpawnScheduler<THandle> {

    protected:

         /**
          * @brief Map from spawn profile IDs to their spawn rules.
          */
         std::unordered_map<helios::gameplay::spawn::types::SpawnProfileId,
                            std::unique_ptr<helios::gameplay::spawn::policy::SpawnRule<THandle>>>
                 spawnRules_;

        /**
         * @brief Map from spawn rule IDs to their runtime state.
         */
        std::unordered_map<
            helios::gameplay::spawn::types::SpawnRuleId,
            helios::gameplay::spawn::policy::SpawnRuleState
        > spawnRuleStates_;

        /**
         * @brief Processor for evaluating individual rules.
         */
        DefaultRuleProcessor<THandle> ruleProcessor_{};

    public:

        /**
         * @brief Constructs a DefaultSpawnScheduler with optional initial capacity.
         *
         * @param initialSpanPlanSize Initial capacity for the spawn plan buffer.
         */
        DefaultSpawnScheduler(const size_t initialSpanPlanSize = 20) {
            SpawnScheduler<THandle>::scheduledSpawnPlans_.reserve(initialSpanPlanSize);
        }

        /**
         * @brief Evaluates all registered spawn rules.
         *
         * @details Iterates through all rules, processes each one, and
         * collects spawn plans for rules whose conditions are met.
         *
         * @param gameWorld The game world where evaluation takes place.
         * @param updateContext Current frame context with delta time and world.
         * @param spawnContext Context for spawn operations.
         */
        void evaluate(
            const GameWorld& gameWorld,
            const UpdateContext& updateContext,
            const SpawnContext<THandle>& spawnContext) noexcept override{

            SpawnScheduler<THandle>::scheduledSpawnPlans_.clear();

            for (auto& [spawnProfileId, rule] : spawnRules_) {

                auto spawnPlan = ruleProcessor_.processRule(
                    gameWorld, updateContext, spawnContext, spawnProfileId, *rule,
                    spawnRuleStates_[rule->spawnRuleId()]
                );

                if (spawnPlan.amount > 0) {
                    SpawnScheduler<THandle>::scheduledSpawnPlans_.push_back({
                        spawnProfileId,
                        std::move(spawnPlan),
                        spawnContext
                    });
                }
            }
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
        DefaultSpawnScheduler& addRule(
            const helios::gameplay::spawn::types::SpawnProfileId spawnProfileId,
            std::unique_ptr<helios::gameplay::spawn::policy::SpawnRule<THandle>> spawnRule
        ) {
            assert(!spawnRules_.contains(spawnProfileId) && "Duplicate SpawnProfile entry");
            assert(!spawnRuleStates_.contains(spawnRule->spawnRuleId()) && "Duplicate SpawnRuleId entry");

            spawnRuleStates_.try_emplace(spawnRule->spawnRuleId());
            spawnRules_.try_emplace(spawnProfileId, std::move(spawnRule));


            return *this;
        }

        /**
         * @brief Commits a completed spawn to update rule state.
         *
         * @details Called after entities are spawned to update the rule's
         * internal state (e.g., spawn count tracking, timer reset).
         *
         * @param spawnRuleId The rule that triggered the spawn.
         * @param spawnCount Number of entities actually spawned.
         */
        void commit(const helios::gameplay::spawn::types::SpawnRuleId spawnRuleId, const size_t spawnCount) noexcept override{

            for (auto& spawnRule : spawnRules_ | std::views::values) {

                if (spawnRule->spawnRuleId() == spawnRuleId) {

                    auto it = spawnRuleStates_.find(spawnRuleId);
                    assert(it != spawnRuleStates_.end() && "Unexpected missing spawnRuleState");

                    spawnRule->commit(it->second, spawnCount);
                }
            }
        }

        /**
         * @brief Resets all rule states to their initial values.
         *
         * @details Iterates through all rules and resets their state.
         */
        void reset() noexcept override {
            for (auto& spawnRule : spawnRules_ | std::views::values) {

                auto it = spawnRuleStates_.find(spawnRule->spawnRuleId());
                assert(it != spawnRuleStates_.end() && "Unexpected missing spawnRuleState");

                spawnRule->reset(it->second);

            }
        }

    };

}
