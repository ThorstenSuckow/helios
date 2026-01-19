/**
 * @file TimerSpawnCondition.ixx
 * @brief Time-based spawn condition for interval-controlled spawning.
 */
module;

#include <cassert>

export module helios.engine.runtime.spawn.policy.conditions.TimerSpawnCondition;

import helios.engine.runtime.spawn.policy.SpawnCondition;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.pooling.GameObjectPoolSnapshot;
import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::spawn::policy::conditions {

    /**
     * @brief A SpawnCondition that triggers spawning at fixed time intervals.
     *
     * @details TimerSpawnCondition checks if enough time has elapsed since the
     * last spawn to satisfy the interval requirement. It also verifies that the
     * pool has sufficient inactive entities to fulfill the requested amount.
     *
     * ## Behavior
     *
     * - **isSatisfied():** Returns true when `sinceLastSpawn >= interval` AND
     *   the pool has enough inactive entities
     * - **onCommit():** Resets the `sinceLastSpawn` timer to 0
     *
     * Example:
     * ```cpp
     * // Spawn every 2 seconds
     * auto condition = std::make_unique<TimerSpawnCondition>(2.0f);
     *
     * auto rule = std::make_unique<SpawnRule>(
     *     std::move(condition),
     *     std::make_unique<FixedSpawnAmount>(3),
     *     SpawnRuleId{1}
     * );
     * ```
     *
     * @see SpawnCondition
     * @see SpawnRule
     * @see SpawnRuleState
     */
    class TimerSpawnCondition : public SpawnCondition {

    private:

        /**
         * @brief The time interval between spawns in seconds.
         */
        float interval_ = 0.0f;

    public:

        /**
         * @brief Constructs a TimerSpawnCondition with the specified interval.
         *
         * @details The first spawn will occur after the interval has elapsed,
         * not immediately.
         *
         * @param interval The time in seconds between spawn events.
         *
         * @pre interval > 0.0f
         */
        explicit TimerSpawnCondition(const float interval)
            : interval_(interval) {
            assert(interval > 0.0f && "Interval must be > 0");
        }

        /**
         * @brief Checks if the timer interval has elapsed and pool has capacity.
         *
         * @param requestedAmount The number of entities requested to spawn.
         * @param spawnState The rule's runtime state containing time since last spawn.
         * @param poolSnapshot Snapshot of the pool's current capacity.
         * @param updateContext The current frame's context.
         *
         * @return true if interval elapsed AND pool has enough inactive entities.
         */
        [[nodiscard]] bool isSatisfied(
            const size_t requestedAmount,
            const SpawnRuleState& spawnState,
            const helios::engine::runtime::pooling::GameObjectPoolSnapshot poolSnapshot,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const noexcept override {

            if (requestedAmount <= poolSnapshot.inactiveCount &&
                spawnState.sinceLastSpawn() >= interval_) {
                return true;
            }

            return false;
        }

        /**
         * @brief Resets the spawn timer after a successful spawn.
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

    };

}