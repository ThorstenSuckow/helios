/**
 * @file SpawnSystemFactory.ixx
 * @brief Factory for fluent, ID-centric configuration of the spawn system.
 */
module;

export module helios.gameplay.builder.spawnSystem.SpawnSystemFactory;

import helios.gameplay.builder.spawnSystem.builders.configs.SpawnPoolConfig;

import helios.gameplay.spawn.SpawnManager;
import helios.runtime.pooling.EntityPoolManager;

export namespace helios::gameplay::builder::spawnSystem {

    /**
     * @brief Factory for ID-centric spawn system configuration.
     *
     * @details Entry point for fluent spawn system setup. Returns a
     * SpawnSystemConfigurator whose pool() calls can be chained via
     * commit() / commitCyclic() return values:
     *
     * ## Usage
     *
     * ```cpp
     * SpawnSystemFactory::configure(poolManager, spawnManager)
     *     .pool(ProjectilePoolId, ProjectilePrefabId, 50)
     *         .profile(ProjectileProfileId)
     *             .emitterPlacement()
     *             .done()
     *         .commit()
     *     .pool(EnemyPoolId, EnemyPrefabId, 200)
     *         .profile(EnemyProfileId)
     *             .randomPlacement()
     *             .scheduledBy(EnemyRuleId)
     *                 .timerCondition(5.0f)
     *                 .fixedAmount(1)
     *                 .done()
     *             .done()
     *         .commit();
     * ```
     *
     * @see SpawnPoolConfig
     * @see SpawnProfileConfig
     * @see SpawnRuleConfig
     * @see SpawnSystemConfigurator
     */
    template<typename THandle, typename TWorld>
    class SpawnSystemFactory {

    public:

        /**
         * @brief Creates a configurator for fluent spawn system setup.
         *
         * @param poolManager The pool manager for pool registration.
         * @param spawnManager The spawn manager for profile and scheduler registration.
         *
         * @return A SpawnSystemConfigurator for chained pool() calls.
         */
        static helios::gameplay::builder::spawnSystem::builders::configs::SpawnSystemConfigurator<THandle, TWorld> configure(
            helios::runtime::pooling::EntityPoolManager<THandle>& poolManager,
            helios::gameplay::spawn::SpawnManager<THandle, TWorld>& spawnManager
        ) {
            return helios::gameplay::builder::spawnSystem::builders::configs::SpawnSystemConfigurator<THandle, TWorld>{
                poolManager, spawnManager
            };
        }
    };

}






