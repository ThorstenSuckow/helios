/**
 * @file SpawnAll.ixx
 * @brief Spawn amount provider that spawns all available pool objects.
 */
module;


#include <cstddef>
export module helios.gameplay.spawn.policy.amount.SpawnAll;

import helios.runtime.pooling.types.GameObjectPoolId;

import helios.gameplay.spawn.policy.amount.SpawnAmountProvider;
import helios.gameplay.spawn.policy.SpawnRuleState;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameWorld;

import helios.runtime.pooling.GameObjectPoolManager;

export namespace helios::gameplay::spawn::policy::amount {

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
    template<typename THandle>
    class SpawnAll : public SpawnAmountProvider<THandle> {

    public:

        /**
         * @copydoc SpawnAmountProvider::getAmount
         */
        [[nodiscard]] size_t getAmount(
            const helios::runtime::pooling::types::GameObjectPoolId gameObjectPoolId,
            const SpawnRuleState& spawnRuleState,
            const helios::runtime::world::GameWorld& gameWorld,
            const helios::runtime::world::UpdateContext& updateContext
        ) const override {
            const auto* manager = gameWorld.tryManager<helios::runtime::pooling::GameObjectPoolManager<THandle>>();
            if (!manager) {
                return 0;
            }
            return manager->poolSnapshot(gameObjectPoolId).inactiveCount;
        }

    };


}