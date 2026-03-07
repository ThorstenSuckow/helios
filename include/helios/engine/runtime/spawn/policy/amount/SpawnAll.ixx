/**
 * @file SpawnAll.ixx
 * @brief Spawn amount provider that spawns all available pool objects.
 */
module;

export module helios.engine.runtime.spawn.policy.amount.SpawnAll;

import helios.engine.runtime.pooling.types.GameObjectPoolId;

import helios.engine.runtime.spawn.policy.amount.SpawnAmountProvider;
import helios.engine.runtime.spawn.policy.SpawnRuleState;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.pooling.GameObjectPoolManager;

export namespace helios::engine::runtime::spawn::policy::amount {

    /**
     * @brief Spawns all inactive objects from a pool.
     *
     * This provider queries the GameObjectPoolManager to determine
     * how many inactive objects are available in the specified pool
     * and returns that count as the spawn amount.
     *
     * Useful for scenarios where all pooled objects should be
     * spawned at once, such as initial wave spawning or respawning
     * all enemies after a level reset.
     */
    class SpawnAll : public SpawnAmountProvider {

    public:

        /**
         * @copydoc SpawnAmountProvider::getAmount
         */
        [[nodiscard]] size_t getAmount(
            const helios::engine::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::engine::runtime::world::GameWorld& gameWorld,
            const helios::engine::runtime::world::UpdateContext& updateContext
        ) const override {
            const auto* manager = gameWorld.tryManager<helios::engine::runtime::pooling::GameObjectPoolManager>();
            if (!manager) {
                return 0;
            }
            return manager->poolSnapshot(gameObjectPoolId).inactiveCount;
        }

    };


}