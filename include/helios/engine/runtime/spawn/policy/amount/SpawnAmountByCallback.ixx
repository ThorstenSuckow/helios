/**
 * @file SpawnAmountByCallback.ixx
 * @brief Amount provider that delegates to a user-provided callback.
 */
module;

#include <functional>

export module helios.engine.runtime.spawn.policy.amount.SpawnAmountByCallback;

import helios.engine.core.data.GameObjectPoolId;
import helios.engine.runtime.spawn.policy.amount.SpawnAmountProvider;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.world.UpdateContext;

export namespace helios::engine::runtime::spawn::policy::amount {

    /**
     * @brief Amount provider that delegates to a user-provided callback.
     *
     * @details SpawnAmountByCallback allows dynamic spawn quantity calculation
     * by delegating to a std::function. This enables complex logic based on
     * game state, difficulty, pool availability, or other runtime factors.
     *
     * Example:
     * ```cpp
     * auto dynamicAmount = std::make_unique<SpawnAmountByCallback>(
     *     [&DIFFICULTY_LEVEL](const GameObjectPoolId& poolId, const SpawnRuleState& state,
     *        const UpdateContext& ctx) -> size_t {
     *         // Scale spawn count with difficulty
     *         return 2 * DIFFICULTY_LEVEL;
     *     }
     * );
     * ```
     *
     * @see SpawnAmountProvider
     * @see FixedSpawnAmount
     */
    class SpawnAmountByCallback : public SpawnAmountProvider {

        /**
         * @brief Function signature for amount evaluation.
         */
        using AmountEvaluator = std::function<size_t(
            const helios::engine::core::data::GameObjectPoolId,
            const SpawnRuleState&,
            const helios::engine::runtime::world::UpdateContext&
        )>;

        /**
         * @brief The callback function for calculating spawn amount.
         */
        AmountEvaluator evaluator_;

    public:

        /**
         * @brief Constructs a SpawnAmountByCallback with the given evaluator.
         *
         * @param evaluator The callback function to calculate spawn amount.
         */
        explicit SpawnAmountByCallback(AmountEvaluator evaluator)
            : evaluator_(std::move(evaluator)) {}

        /**
         * @brief Returns the spawn amount by invoking the callback.
         *
         * @param gameObjectPoolId The pool to spawn from.
         * @param spawnRuleState The rule's runtime state.
         * @param updateContext The current frame's context.
         *
         * @return The calculated spawn amount.
         */
        [[nodiscard]] size_t getAmount(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const override {
            return evaluator_(gameObjectPoolId, spawnRuleState, updateContext);
        }

    };


}