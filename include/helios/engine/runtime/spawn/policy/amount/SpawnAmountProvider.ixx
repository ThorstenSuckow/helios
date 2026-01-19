/**
 * @file SpawnAmountProvider.ixx
 * @brief Abstract interface for determining spawn quantity.
 */
module;

export module helios.engine.runtime.spawn.policy.amount.SpawnAmountProvider;

import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::spawn::policy::amount {

    /**
     * @brief Abstract interface for determining spawn quantity.
     *
     * @details SpawnAmountProvider defines the strategy for calculating how
     * many entities should spawn when a rule's condition is satisfied.
     *
     * Implementations include:
     * - **FixedSpawnAmount:** Always returns a fixed count
     * - **SpawnAmountByCallback:** Delegates to a user-provided function
     *
     * @see FixedSpawnAmount
     * @see SpawnAmountByCallback
     * @see SpawnRule
     */
    class SpawnAmountProvider {

    public:

        virtual ~SpawnAmountProvider() = default;

        /**
         * @brief Returns the number of entities to spawn.
         *
         * @param gameObjectPoolId The pool to spawn from.
         * @param spawnRuleState The rule's current runtime state.
         * @param updateContext The current frame's context.
         *
         * @return Number of entities to spawn.
         */
        [[nodiscard]] virtual size_t getAmount(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const = 0;

    };


}