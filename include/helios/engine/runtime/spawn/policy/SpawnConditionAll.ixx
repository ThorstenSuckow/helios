/**
 * @file SpawnConditionAll.ixx
 * @brief Composite condition that requires all child conditions to be satisfied.
 */
module;

#include <memory>
#include <vector>


export module helios.engine.runtime.spawn.policy.SpawnConditionAll;

import helios.engine.runtime.spawn.policy.SpawnCondition;

import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.pooling.GameObjectPoolSnapshot;
import helios.engine.runtime.world.UpdateContext;


export namespace helios::engine::runtime::spawn::policy {

    /**
     * @brief Composite condition requiring all child conditions to pass.
     *
     * @details SpawnConditionAll implements a logical AND over multiple
     * SpawnCondition instances. The spawn is permitted only when every
     * child condition returns `true` from `isSatisfied()`.
     *
     * ## Behavior
     *
     * - Short-circuits on first failing condition
     * - Propagates `onCommit()` to all child conditions
     * - Accepts variadic condition arguments via constructor
     *
     * ## Usage
     *
     * ```cpp
     * auto condition = std::make_unique<SpawnConditionAll>(
     *     std::make_unique<TimerCondition>(2.0f),
     *     std::make_unique<RequestedAmountIsAvailableCondition>(),
     *     std::make_unique<MaxActiveCondition>(10)
     * );
     *
     * // Spawns only when: timer elapsed AND pool available AND under max
     * ```
     *
     * @see SpawnCondition
     * @see TimerCondition
     * @see RequestedAmountIsAvailableCondition
     */
    class SpawnConditionAll : public SpawnCondition {

        /**
         * @brief Collection of child conditions to evaluate.
         */
        std::vector<std::unique_ptr<helios::engine::runtime::spawn::policy::SpawnCondition>> spawnConditions_;

    public:

        /**
         * @brief Constructs a composite condition from variadic arguments.
         *
         * @tparam Args Types of spawn conditions (must be unique_ptr<SpawnCondition>).
         *
         * @param args Spawn conditions to combine. Ownership transferred.
         */
        template<typename... Args>
        explicit SpawnConditionAll(Args&&... args) {

            spawnConditions_.reserve(10);

            (spawnConditions_.push_back(std::forward<Args>(args)), ...);

        }

        /**
         * @brief Checks if all child conditions are satisfied.
         *
         * @details Iterates through all conditions and returns `false` on the
         * first condition that fails (short-circuit evaluation).
         *
         * @param requestedAmount Number of entities requested for spawn.
         * @param spawnState Current state of the spawn rule.
         * @param poolSnapshot Snapshot of the entity pool.
         * @param updateContext Current frame context.
         *
         * @return `true` if all conditions pass, `false` otherwise.
         */
        [[nodiscard]] bool isSatisfied(
            const size_t requestedAmount,
            const SpawnRuleState& spawnState,
            const helios::engine::runtime::pooling::GameObjectPoolSnapshot poolSnapshot,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const noexcept override {


            for (auto& condition : spawnConditions_) {
                if (!condition->isSatisfied(requestedAmount, spawnState, poolSnapshot, updateContext)) {
                    return false;
                }
            }

            return true;

        };

        /**
         * @brief Propagates commit to all child conditions.
         *
         * @details Calls `onCommit()` on each child condition to allow them
         * to update their internal state after a successful spawn.
         *
         * @param spawnRuleState The rule's runtime state to update.
         * @param spawnAmount The number of entities that were spawned.
         */
        void onCommit(
            SpawnRuleState& spawnRuleState,
            const size_t spawnAmount
        ) const override {
            for (auto& condition : spawnConditions_) {
                condition->onCommit(spawnRuleState, spawnAmount);
            }

        }

        /**
         * @brief Propagates reset to all child conditions.
         *
         * @param spawnRuleState The rule's runtime state to reset.
         */
        void onReset(
            SpawnRuleState& spawnRuleState
        ) const noexcept override {
            for (auto& condition : spawnConditions_) {
                condition->onReset(spawnRuleState);
            }

        }


    };

}