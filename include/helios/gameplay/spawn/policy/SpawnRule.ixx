/**
 * @file SpawnRule.ixx
 * @brief Combines a condition and amount provider into a complete spawn rule.
 */
module;

#include <memory>
#include <cstddef>

export module helios.gameplay.spawn.policy.SpawnRule;

import helios.gameplay.spawn.policy.SpawnCondition;
import helios.gameplay.spawn.policy.amount.SpawnAmountProvider;
import helios.gameplay.spawn.policy.SpawnRuleState;
import helios.gameplay.spawn.scheduling.SpawnPlan;
import helios.runtime.pooling.GameObjectPoolSnapshot;
import helios.runtime.world;
import helios.runtime.pooling.types.GameObjectPoolId;
import helios.gameplay.spawn.types.SpawnRuleId;

export namespace helios::gameplay::spawn::policy {

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
    template<typename THandle>
    class SpawnRule {

        /**
         * @brief Provider that determines spawn quantity.
         */
        std::unique_ptr<const amount::SpawnAmountProvider<THandle>> spawnAmountProvider_;

        /**
         * @brief Condition that determines if spawning should occur.
         */
        std::unique_ptr<const SpawnCondition> spawnCondition_;

        /**
         * @brief Unique identifier for this rule.
         */
        const helios::gameplay::spawn::types::SpawnRuleId spawnRuleId_;

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
            std::unique_ptr<const amount::SpawnAmountProvider<THandle>> spawnAmountProvider,
            const helios::gameplay::spawn::types::SpawnRuleId spawnRuleId
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
        void commit(SpawnRuleState& spawnRuleState, const size_t spawnAmount) const {
            spawnCondition_->onCommit(spawnRuleState, spawnAmount);
        }

        /**
         * @brief Resets the rule's condition state.
         *
         * @param spawnRuleState The state to reset.
         */
        void reset(SpawnRuleState& spawnRuleState) const{
            spawnCondition_->onReset(spawnRuleState);
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
         * @param gameWorld The game world where the spawn plan is executed.
         * @param updateContext The current frame's context.
         *
         * @return A SpawnPlan with amount > 0 if condition satisfied, 0 otherwise.
         */
        [[nodiscard]] helios::gameplay::spawn::scheduling::SpawnPlan evaluate(
            const helios::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const helios::runtime::pooling::GameObjectPoolSnapshot& poolSnapshot,
            const SpawnRuleState& spawnRuleState,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext& updateContext
        ) {
            auto amount = spawnAmountProvider_->getAmount(gameObjectPoolId, spawnRuleState, gameWorld, updateContext);

            if (spawnCondition_->isSatisfied(amount, spawnRuleState, poolSnapshot, updateContext)) {
                return helios::gameplay::spawn::scheduling::SpawnPlan{spawnRuleId_, amount};
            }

            return helios::gameplay::spawn::scheduling::SpawnPlan{spawnRuleId_, 0};
        }

        /**
         * @brief Returns the rule's unique identifier.
         *
         * @return The SpawnRuleId.
         */
        [[nodiscard]] helios::gameplay::spawn::types::SpawnRuleId spawnRuleId() const noexcept {
            return spawnRuleId_;
        }

    };

}