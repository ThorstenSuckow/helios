/**
 * @file SpawnAmountByCallback.ixx
 * @brief Amount provider that delegates to a user-provided callback.
 */
module;

#include <functional>
#include <cstddef>

export module helios.gameplay.spawn.policy.amount.SpawnAmountByCallback;

import helios.runtime.pooling.types.GameObjectPoolId;
import helios.gameplay.spawn.policy.amount.SpawnAmountProvider;
import helios.gameplay.spawn.policy.SpawnRuleState;
import helios.runtime.world;

export namespace helios::gameplay::spawn::policy::amount {

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
    template<typename THandle>
    class SpawnAmountByCallback : public SpawnAmountProvider<THandle> {

        /**
         * @brief Function signature for amount evaluation.
         */
        using AmountEvaluator = std::function<size_t(
            const helios::runtime::pooling::types::GameObjectPoolId,
            const SpawnRuleState&,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext&
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
         * @copydoc SpawnAmountProvider::getAmount
         */
        [[nodiscard]] size_t getAmount(
            const helios::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext& updateContext
        ) const override {
            return evaluator_(gameObjectPoolId, spawnRuleState, gameWorld, updateContext);
        }

    };


}