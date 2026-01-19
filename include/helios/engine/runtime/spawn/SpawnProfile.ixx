/**
 * @file SpawnProfile.ixx
 * @brief Configuration profile defining how entities are spawned and initialized.
 */
module;

#include <memory>

export module helios.engine.runtime.spawn.SpawnProfile;

import helios.engine.runtime.spawn.behavior.SpawnPlacer;
import helios.engine.runtime.spawn.behavior.SpawnInitializer;
import helios.engine.core.data.GameObjectPoolId;

export namespace helios::engine::runtime::spawn {

    /**
     * @brief Configuration profile defining spawn behavior for a pool.
     *
     * @details SpawnProfile bundles a pool ID with the behaviors that control
     * how spawned entities are positioned and initialized. Each profile is
     * registered with a SpawnProfileId in the SpawnManager.
     *
     * A profile consists of:
     * - **Pool ID:** Which pool to acquire entities from
     * - **Placer:** Determines spawn position (random, formation, emitter-relative)
     * - **Initializer:** Configures entity state (velocity, direction, stats)
     *
     * Example:
     * ```cpp
     * auto profile = std::make_unique<SpawnProfile>(SpawnProfile{
     *     .gameObjectPoolId = bulletPoolId,
     *     .spawnPlacer = std::make_unique<EmitterRelativePlacer>(),
     *     .spawnInitializer = std::make_unique<ProjectileInitializer>(speed)
     * });
     * spawnManager.addSpawnProfile(profileId, std::move(profile));
     * ```
     *
     * @see SpawnPlacer
     * @see SpawnInitializer
     * @see SpawnManager
     */
    struct SpawnProfile {

        /**
         * @brief The pool from which entities are acquired.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId;

        /**
         * @brief Strategy for determining spawn positions.
         */
        std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnPlacer> spawnPlacer;

        /**
         * @brief Strategy for initializing spawned entity state.
         */
        std::unique_ptr<helios::engine::runtime::spawn::behavior::SpawnInitializer> spawnInitializer;

    };

}