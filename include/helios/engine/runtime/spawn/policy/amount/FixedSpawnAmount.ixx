/**
 * @file FixedSpawnAmount.ixx
 * @brief Amount provider that returns a fixed spawn count.
 */
module;

export module helios.engine.runtime.spawn.policy.amount.FixedSpawnAmount;

import helios.engine.core.data.GameObjectPoolId;
import helios.engine.runtime.spawn.policy.amount.SpawnAmountProvider;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::spawn::policy::amount {

    /**
     * @brief Amount provider that returns a fixed spawn count.
     *
     * @details FixedSpawnAmount always returns the same number of entities
     * to spawn, regardless of game state. Useful for simple, predictable
     * spawn patterns.
     *
     * @see SpawnAmountProvider
     */
    class FixedSpawnAmount : public SpawnAmountProvider {

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
         * @brief Returns the fixed spawn amount.
         *
         * @param gameObjectPoolId The pool to spawn from (unused).
         * @param spawnRuleState The rule's runtime state (unused).
         * @param updateContext The current frame's context (unused).
         *
         * @return The fixed spawn amount.
         */
        [[nodiscard]] size_t getAmount(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const override {
            return amount_;
        }

    };


}