/**
 * @file FixedSpawnAmount.ixx
 * @brief Amount provider that returns a fixed spawn count.
 */
module;

export module helios.runtime.spawn.policy.amount.FixedSpawnAmount;

import helios.runtime.pooling.types.GameObjectPoolId;
import helios.runtime.spawn.policy.amount.SpawnAmountProvider;
import helios.runtime.spawn.policy.SpawnRuleState;
import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

export namespace helios::runtime::spawn::policy::amount {

    /**
     * @brief Amount provider that returns a fixed spawn count.
     *
     * @details FixedSpawnAmount always returns the same number of entities
     * to spawn, regardless of game state. Useful for simple, predictable
     * spawn patterns.
     *
     * @see SpawnAmountProvider
     */
    template<typename THandle>
    class FixedSpawnAmount : public SpawnAmountProvider<THandle> {

        /**
         * @brief The fixed number of entities to spawn.
         */
        size_t amount_;

    public:

        /**
         * @brief Constructs a FixedSpawnAmount with the specified count.
         *
         * @param amount The number of entities to spawn each time.
         */
        explicit FixedSpawnAmount(const size_t amount)
            : amount_(amount) {}

        /**
         * @copydoc SpawnAmountProvider::getAmount
         */
        [[nodiscard]] size_t getAmount(
            const helios::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext& updateContext
        ) const override {
            return amount_;
        }

    };


}