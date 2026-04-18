/**
 * @file DefaultRuleProcessor.ixx
 * @brief Default implementation of RuleProcessor for spawn rule evaluation.
 */
module;

#include <cassert>
#include <queue>


export module helios.engine.runtime.spawn.scheduling.DefaultRuleProcessor;

import helios.engine.runtime.spawn.scheduling.RuleProcessor;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.spawn.SpawnManager;
import helios.engine.runtime.spawn.types.SpawnContext;
import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.spawn.scheduling.SpawnPlan;
import helios.engine.runtime.spawn.scheduling.ScheduledSpawnPlan;
import helios.engine.runtime.spawn.types;
import helios.engine.runtime.spawn.policy.SpawnRule;
import helios.engine.runtime.spawn.policy.SpawnRuleState;
import helios.engine.runtime.pooling.GameObjectPoolManager;

using namespace helios::engine::runtime::spawn::policy;
using namespace helios::engine::runtime::spawn::types;
using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::spawn::scheduling {

    /**
     * @brief Default implementation of RuleProcessor.
     *
     * @details DefaultRuleProcessor provides the standard rule processing logic:
     *
     * 1. Retrieves the SpawnProfile from the SpawnManager
     * 2. Gets a pool snapshot from the GameObjectPoolManager
     * 3. Updates the rule state with the current delta time
     * 4. Evaluates the rule and returns the resulting SpawnPlan
     *
     * This processor is used by both DefaultSpawnScheduler and CyclicSpawnScheduler.
     *
     * @see RuleProcessor
     * @see DefaultSpawnScheduler
     * @see CyclicSpawnScheduler
     */
    template<typename THandle>
    class DefaultRuleProcessor : public RuleProcessor<THandle> {

    public:

        /**
         * @brief Processes a spawn rule using standard evaluation logic.
         *
         * @details Retrieves pool state, updates the rule's timer/state,
         * and evaluates the rule to produce a spawn plan.
         *
         * @param gameWorld The game world where the rule is processed.
         * @param updateContext Current frame context.
         * @param spawnContext Context for the spawn operation.
         * @param spawnProfileId Profile ID to look up spawn configuration.
         * @param spawnRule The rule to evaluate.
         * @param spawnRuleState Mutable state tracking timers and counts.
         *
         * @return SpawnPlan indicating how many entities to spawn.
         */
        SpawnPlan processRule(
            const world::GameWorld& gameWorld,
            const UpdateContext& updateContext,
            const SpawnContext<THandle>& spawnContext,
            const SpawnProfileId spawnProfileId,
            SpawnRule<THandle>& spawnRule,
            SpawnRuleState& spawnRuleState
        ) noexcept override {
            const auto* poolManager  = gameWorld.tryManager<helios::engine::runtime::pooling::GameObjectPoolManager<THandle>>();
            const auto* spawnManager = gameWorld.tryManager<helios::engine::runtime::spawn::SpawnManager<THandle>>();

            const auto* spawnProfile = spawnManager->spawnProfile(spawnProfileId);
            assert(spawnProfile != nullptr);

            const auto& [gameObjectPoolId, _, __] = *spawnProfile;

            const auto poolSnapshot = poolManager->poolSnapshot(gameObjectPoolId);

            // tick the rule state
            spawnRuleState.update(updateContext.deltaTime());

            return spawnRule.evaluate(
                gameObjectPoolId, poolSnapshot,
                spawnRuleState,
                gameWorld,
                updateContext
            );
        }

    };

}
