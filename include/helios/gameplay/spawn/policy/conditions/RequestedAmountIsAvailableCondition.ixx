/**
 * @file RequestedAmountIsAvailableCondition.ixx
 * @brief Spawn condition checking pool availability for requested amount.
 */
module;

#include <cassert>
#include <cstddef>

export module helios.gameplay.spawn.policy.conditions.RequestedAmountIsAvailableCondition;

import helios.gameplay.spawn.policy.SpawnCondition;
import helios.gameplay.spawn.policy.SpawnRuleState;
import helios.runtime.pooling.EntityPoolSnapshot;
import helios.runtime.world.UpdateContext;

export namespace helios::gameplay::spawn::policy::conditions {

    /**
     * @brief Condition verifying the pool has enough inactive entities.
     *
     * @details RequestedAmountIsAvailableCondition checks whether the entity
     * pool contains at least as many inactive (recyclable) entities as the
     * spawn request requires. This prevents spawn failures due to pool
     * exhaustion.
     *
     * ## Behavior
     *
     * - Returns `true` if `requestedAmount <= poolSnapshot.inactiveCount`
     * - Resets the spawn timer on commit (via `setSinceLastSpawn(0)`)
     *
     * ## Usage
     *
     * ```cpp
     * auto condition = std::make_unique<RequestedAmountIsAvailableCondition>();
     *
     * // Often combined with other conditions
     * auto combined = std::make_unique<SpawnConditionAll>(
     *     std::make_unique<TimerCondition>(2.0f),
     *     std::make_unique<RequestedAmountIsAvailableCondition>()
     * );
     * ```
     *
     * @see SpawnCondition
     * @see SpawnConditionAll
     * @see EntityPoolSnapshot
     */
    class RequestedAmountIsAvailableCondition : public SpawnCondition {

    public:

        /**
         * @brief Checks if the pool has enough inactive entities.
         *
         * @param requestedAmount Number of entities requested for spawn.
         * @param spawnState Current state of the spawn rule (unused).
         * @param poolSnapshot Snapshot containing pool availability info.
         * @param updateContext Current frame context (unused).
         *
         * @return `true` if inactive count >= requested amount.
         */
        [[nodiscard]] bool isSatisfied(
            const size_t requestedAmount,
            const SpawnRuleState& spawnState,
            const helios::runtime::pooling::EntityPoolSnapshot poolSnapshot,
            const helios::runtime::world::UpdateContext& updateContext
        ) const noexcept override {
            return requestedAmount <= poolSnapshot.inactiveCount;
        }

        /**
         * @brief Resets the spawn timer after a successful spawn.
         *
         * @details Sets `sinceLastSpawn` to zero, restarting the timer for
         * conditions that depend on spawn intervals.
         *
         * @param spawnRuleState The rule's runtime state to update.
         * @param spawnAmount The number of entities that were spawned (unused).
         */
        void onCommit(
            SpawnRuleState& spawnRuleState,
            const size_t spawnAmount
        ) const override {
            spawnRuleState.setSinceLastSpawn(0.0f);
        }

        /**
         * @brief Resets the spawn timer.
         *
         * @param spawnRuleState The rule's runtime state to reset.
         */
        void onReset(
            SpawnRuleState& spawnRuleState
        ) const noexcept override {
            spawnRuleState.setSinceLastSpawn(0.0f);
        }

    };

}
