/**
 * @file SpawnAmountProvider.ixx
 * @brief Abstract interface for determining spawn quantity.
 */
module;

export module helios.gameplay.spawn.policy.amount.SpawnAmountProvider;

import helios.gameplay.spawn.policy.SpawnRuleState;
import helios.runtime.pooling.types.GameObjectPoolId;
import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;

export namespace helios::gameplay::spawn::policy::amount {

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
    template<typename THandle>
    class SpawnAmountProvider {

    public:

        virtual ~SpawnAmountProvider() = default;

        /**
         * @brief Returns the number of entities to spawn.
         *
         * @param gameObjectPoolId The pool to spawn from.
         * @param spawnRuleState The rule's current runtime state.
         * @param gameWorld The game world in which the spawn occurs.
         * @param updateContext The current frame's context.
         *
         * @return Number of entities to spawn.
         */
        [[nodiscard]] virtual size_t getAmount(
            const helios::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext& updateContext
        ) const = 0;

    };


}