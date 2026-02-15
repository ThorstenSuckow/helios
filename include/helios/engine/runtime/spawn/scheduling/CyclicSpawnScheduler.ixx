/**
 * @file CyclicSpawnScheduler.ixx
 * @brief Scheduler that cycles through spawn rules in round-robin order.
 */
module;

#include <cassert>
#include <memory>
#include <queue>
#include <ranges>
#include <unordered_map>
#include <vector>
#include <algorithm>

export module helios.engine.runtime.spawn.scheduling.CyclicSpawnScheduler;

import helios.engine.runtime.spawn.scheduling.SpawnScheduler;
import helios.engine.runtime.spawn.scheduling.DefaultRuleProcessor;

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

import helios.core.types;

export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief Configuration pairing a spawn profile with its rule.
     */
    struct RuleConfig {

        /**
         * @brief The spawn profile ID for this rule.
         */
        helios::engine::core::data::SpawnProfileId spawnProfileId{helios::core::types::no_init};

        /**
         * @brief The spawn rule controlling when spawns occur.
         */
        std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule> spawnRule;
    };

    /**
     * @brief Scheduler that cycles through spawn rules in round-robin order.
     *
     * @details CyclicSpawnScheduler evaluates one rule per frame in a fixed-size
     * ring buffer, advancing only when a spawn successfully occurs. This creates
     * predictable, sequential spawn patterns ideal for wave-based gameplay.
     *
     * ## Behavior
     *
     * - Evaluates the current rule each `evaluate()` call
     * - Advances cursor only when a spawn plan is successfully produced
     * - Rules that don't trigger are re-evaluated on subsequent frames
     * - Uses compile-time fixed capacity via template parameter
     *
     * ## Use Cases
     *
     * - **Wave spawning:** Sequential enemy types, each wave completes before next
     * - **Boss patterns:** Predictable attack phase cycling
     * - **Resource spawning:** Alternating pickup types
     *
     * ## Usage
     *
     * ```cpp
     * CyclicSpawnScheduler<3> scheduler;
     * scheduler
     *     .addRule(enemyAProfileId, std::make_unique<TimerSpawnRule>(ruleA, 2.0f, 1))
     *     .addRule(enemyBProfileId, std::make_unique<TimerSpawnRule>(ruleB, 2.0f, 1))
     *     .addRule(bossProfileId, std::make_unique<TimerSpawnRule>(ruleC, 5.0f, 1));
     *
     * // Evaluates ruleA until spawn occurs, then advances to ruleB, etc.
     * // Cycle: A → B → Boss → A → B → Boss → ...
     * ```
     *
     * @tparam N Maximum number of rules the scheduler can hold.
     *
     * @see SpawnScheduler
     * @see DefaultSpawnScheduler
     * @see RuleConfig
     */
    template<std::size_t N>
    class CyclicSpawnScheduler : public SpawnScheduler {

        /**
         * @brief Fixed-size ring buffer of rule configurations.
         */
        std::array<RuleConfig, N> ringBuffer_{};

        /**
         * @brief Current position in the ring buffer.
         */
        size_t cursor_ = 0;

        /**
         * @brief Number of rules currently registered.
         */
        size_t count_ = 0;

        /**
         * @brief Map from spawn rule IDs to their runtime state.
         */
        std::unordered_map<
            helios::engine::core::data::SpawnRuleId,
            helios::engine::runtime::spawn::policy::SpawnRuleState
        > spawnRuleStates_;

        /**
         * @brief Processor for evaluating individual rules.
         */
        DefaultRuleProcessor ruleProcessor_;

    public:

        /**
         * @brief Evaluates the current rule in the cycle.
         *
         * @details Processes only the rule at the current cursor position.
         * If the rule produces a spawn plan, the cursor advances to the next rule.
         * Otherwise, the same rule is evaluated again next frame.
         *
         * @param updateContext Current frame context.
         * @param spawnContext Context for spawn operations.
         */
        void evaluate(
            const helios::engine::runtime::world::UpdateContext& updateContext,
            const helios::engine::runtime::spawn::SpawnContext& spawnContext ) noexcept override {

            scheduledSpawnPlans_.clear();

            // Process queue
            auto& [spawnProfileId, spawnRule] = ringBuffer_[cursor_];
            auto spawnPlan = ruleProcessor_.processRule(
                updateContext, spawnContext, spawnProfileId, *spawnRule,
                spawnRuleStates_[spawnRule->spawnRuleId()]);

            if (spawnPlan.amount  > 0) {
                scheduledSpawnPlans_.push_back({
                    spawnProfileId,
                    std::move(spawnPlan),
                    spawnContext
                });
                cursor_ = (cursor_ + 1) % (count_);
            }

        }

        /**
         * @brief Adds a spawn rule to the cycle.
         *
         * @details Appends the rule to the ring buffer. Rules are evaluated
         * in the order they are added.
         *
         * @param spawnProfileId Profile ID for the spawned entities.
         * @param spawnRule The spawn rule. Ownership transferred.
         *
         * @return Reference to this scheduler for chaining.
         *
         * @pre The scheduler has capacity (count_ < N).
         * @pre No duplicate profile IDs or rule IDs.
         */
        CyclicSpawnScheduler& addRule(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnRule> spawnRule
        ) {
            assert(count_ < N);

            assert(!spawnRuleStates_.contains(spawnRule->spawnRuleId()) && "Duplicate SpawnRuleId entry");

            for (const auto& item : ringBuffer_) {
                assert(item.spawnProfileId != spawnProfileId && "Duplicate SpawnProfile entry");
             }

            spawnRuleStates_.try_emplace(spawnRule->spawnRuleId());

            ringBuffer_[count_++] = {
                spawnProfileId, std::move(spawnRule)
            };

            return *this;
        }

        /**
         * @brief Commits a completed spawn to update rule state.
         *
         * @details Called after entities are spawned to update the rule's
         * internal state (e.g., spawn count tracking).
         *
         * @param spawnRuleId The rule that triggered the spawn.
         * @param spawnCount Number of entities actually spawned.
         */
        void commit(const helios::engine::core::data::SpawnRuleId spawnRuleId, const size_t spawnCount) noexcept override{

            for (auto& ruleConfig : ringBuffer_) {

                if (ruleConfig.spawnRule->spawnRuleId() == spawnRuleId) {

                    auto it = spawnRuleStates_.find(spawnRuleId);
                    assert(it != spawnRuleStates_.end() && "Unexpected missing spawnRuleState");

                    ruleConfig.spawnRule->commit(it->second, spawnCount);
                }
            }
        }

        /**
         * @brief Resets all rule states and the cursor to initial values.
         *
         * @details Iterates through all rules and resets their state.
         * Also resets the cursor to the first rule in the cycle.
         */
        void reset() noexcept override {
            for (auto& ruleConfig : ringBuffer_) {

                auto it = spawnRuleStates_.find(ruleConfig.spawnRule->spawnRuleId());
                assert(it != spawnRuleStates_.end() && "Unexpected missing spawnRuleState");

                ruleConfig.spawnRule->reset(it->second);

            }
            cursor_ = 0;
        }

    };

}
