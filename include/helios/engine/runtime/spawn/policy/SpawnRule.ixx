/**
 * @file SpawnRule.ixx
 * @brief Combines a condition and amount provider into a complete spawn rule.
 */
module;

#include <memory>

export module helios.engine.runtime.spawn.policy.SpawnRule;

import helios.engine.runtime.spawn.policy.SpawnCondition;
import helios.engine.runtime.spawn.policy.amount.SpawnAmountProvider;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.runtime.pooling.GameObjectPoolSnapshot;
import helios.engine.runtime.world.UpdateContext;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.core.data.SpawnRuleId;

export namespace helios::engine::runtime::spawn::policy {

    /**
     * @brief Combines a condition and amount provider into a complete spawn rule.
     *
     * @details SpawnRule encapsulates the logic for when and how many entities
     * to spawn. It combines:
     *
     * - **SpawnCondition:** Determines if spawning should occur
     * - **SpawnAmountProvider:** Determines how many to spawn
     *
     * The SpawnScheduler evaluates rules each frame and creates SpawnPlans
     * for rules whose conditions are satisfied.
     *
     * Example:
     * ```cpp
     * auto rule = std::make_unique<SpawnRule>(
     *     std::make_unique<TimerSpawnCondition>(2.0f),
     *     std::make_unique<FixedSpawnAmount>(3),
     *     SpawnRuleId{1}
     * );
     * ```
     *
     * @see SpawnCondition
     * @see SpawnAmountProvider
     * @see SpawnScheduler
     */
    class SpawnRule {

        /**
         * @brief Provider that determines spawn quantity.
         */
        std::unique_ptr<const amount::SpawnAmountProvider> spawnAmountProvider_;

        /**
         * @brief Condition that determines if spawning should occur.
         */
        std::unique_ptr<const SpawnCondition> spawnCondition_;

        /**
         * @brief Unique identifier for this rule.
         */
        const helios::engine::core::data::SpawnRuleId spawnRuleId_;

    public:

        /**
         * @brief Constructs a SpawnRule with condition, amount provider, and ID.
         *
         * @param spawnCondition Condition determining when to spawn.
         * @param spawnAmountProvider Provider determining how many to spawn.
         * @param spawnRuleId Unique identifier for this rule.
         */
        explicit SpawnRule(
            std::unique_ptr<const SpawnCondition> spawnCondition,
            std::unique_ptr<const amount::SpawnAmountProvider> spawnAmountProvider,
            const helios::engine::core::data::SpawnRuleId spawnRuleId
        ) :
            spawnCondition_(std::move(spawnCondition)),
            spawnAmountProvider_(std::move(spawnAmountProvider)),
            spawnRuleId_(spawnRuleId) {}

        /**
         * @brief Commits a completed spawn to update rule state.
         *
         * @param spawnRuleState The state to update.
         * @param spawnAmount The number of entities spawned.
         */
        void commit(SpawnRuleState& spawnRuleState, const size_t spawnAmount) {
            spawnCondition_->onCommit(spawnRuleState, spawnAmount);
        }

        /**
         * @brief Returns the spawn condition.
         *
         * @return Reference to the spawn condition.
         */
        [[nodiscard]] const SpawnCondition& spawnCondition() const noexcept {
            return *spawnCondition_;
        }

        /**
         * @brief Evaluates the rule and returns a spawn plan.
         *
         * @param gameObjectPoolId The pool to spawn from.
         * @param poolSnapshot Current pool state.
         * @param spawnRuleState The rule's runtime state.
         * @param updateContext The current frame's context.
         *
         * @return A SpawnPlan with amount > 0 if condition satisfied, 0 otherwise.
         */
        [[nodiscard]] helios::engine::runtime::spawn::scheduling::SpawnPlan evaluate(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const helios::engine::runtime::pooling::GameObjectPoolSnapshot& poolSnapshot,
            const SpawnRuleState& spawnRuleState,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) {
            auto amount = spawnAmountProvider_->getAmount(gameObjectPoolId, spawnRuleState, updateContext);

            if (spawnCondition_->isSatisfied(amount, spawnRuleState, poolSnapshot, updateContext)) {
                return helios::engine::runtime::spawn::scheduling::SpawnPlan{spawnRuleId_, amount};
            }

            return helios::engine::runtime::spawn::scheduling::SpawnPlan{spawnRuleId_, 0};
        }

        /**
         * @brief Returns the rule's unique identifier.
         *
         * @return The SpawnRuleId.
         */
        [[nodiscard]] helios::engine::core::data::SpawnRuleId spawnRuleId() const noexcept {
            return spawnRuleId_;
        }

    };

}