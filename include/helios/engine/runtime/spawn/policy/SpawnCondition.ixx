/**
 * @file SpawnCondition.ixx
 * @brief Abstract interface for determining spawn timing.
 */
module;

export module helios.engine.runtime.spawn.policy.SpawnCondition;

import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.pooling.GameObjectPoolSnapshot;
import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::spawn::policy {

    /**
     * @brief Abstract interface for determining when spawning should occur.
     *
     * @details SpawnCondition implements the Strategy pattern for spawn timing
     * control. Concrete implementations determine whether spawning should occur
     * based on time, game state, pool availability, or other conditions.
     *
     * ## Responsibilities
     *
     * - **isSatisfied():** Evaluates whether the condition is met for spawning
     * - **onCommit():** Updates state after a successful spawn (e.g., reset timer)
     *
     * ## Implementation Example
     *
     * ```cpp
     * class WaveSpawnCondition : public SpawnCondition {
     * public:
     *     bool isSatisfied(size_t amount, const SpawnRuleState& state,
     *                      GameObjectPoolSnapshot pool,
     *                      const UpdateContext& ctx) const noexcept override {
     *         return state.sinceLastSpawn() >= waveInterval_
     *             && pool.inactiveCount >= amount;
     *     }
     *
     *     void onCommit(SpawnRuleState& state, size_t count) const override {
     *         state.setSinceLastSpawn(0.0f);
     *     }
     * };
     * ```
     *
     * @see TimerSpawnCondition
     * @see SpawnRule
     * @see SpawnScheduler
     */
    class SpawnCondition {

    public:

        /**
         * @brief Virtual destructor for proper polymorphic cleanup.
         */
        virtual ~SpawnCondition() = default;

        /**
         * @brief Evaluates whether the spawn condition is satisfied.
         *
         * @details Called by SpawnRule::evaluate() to determine if spawning
         * should occur. The implementation should check all required conditions
         * (time elapsed, pool capacity, game state, etc.).
         *
         * @param requestedAmount The number of entities requested to spawn.
         * @param spawnState The rule's current runtime state.
         * @param poolSnapshot Snapshot of the pool's current capacity.
         * @param updateContext The current frame's context.
         *
         * @return true if spawning should occur, false otherwise.
         */
        [[nodiscard]] virtual bool isSatisfied(
            const size_t requestedAmount,
            const SpawnRuleState& spawnState,
            const helios::engine::runtime::pooling::GameObjectPoolSnapshot poolSnapshot,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const noexcept = 0;

        /**
         * @brief Called after a successful spawn to update condition state.
         *
         * @details Override this method to reset timers, increment counters,
         * or perform other state updates after spawning completes.
         *
         * The default implementation is a no-op.
         *
         * @param spawnRuleState The rule's runtime state to update.
         * @param spawnAmount The number of entities that were spawned.
         */
        virtual void onCommit(
            SpawnRuleState& spawnRuleState,
            const size_t spawnAmount
        ) const {
            // noop
        }


    };

}