/**
 * @file GameObjectPoolConfig.ixx
 * @brief Configuration data for GameObjectPool initialization.
 */
module;

#include <memory>

export module helios.engine.runtime.pooling.GameObjectPoolConfig;

import helios.engine.ecs.GameObject;
import helios.engine.core.data;

export namespace helios::engine::runtime::pooling {

    /**
     * @brief Configuration structure for creating a GameObjectPool.
     *
     * @details GameObjectPoolConfig bundles all information required to initialize
     * a GameObjectPool: a unique pool identifier, a PrefabId referencing the
     * prefab template, and the initial pool size.
     *
     * The PrefabId identifies the prefab GameObject that serves as the archetype
     * from which all pooled instances are cloned. The prefab is registered
     * separately via GameObjectFactory::withPrefabId() and looked up by the
     * pool system at initialization time.
     *
     * Example usage:
     * ```cpp
     * // Create and tag prefab
     * auto enemyPrefab = GameObjectFactory::instance()
     *     .gameObject(gameWorld)
     *     .withPrefabId(EnemyPrefabId)
     *     // ... component configuration ...
     *     .make();
     *
     * auto config = std::make_unique<GameObjectPoolConfig>(
     *     EnemyPoolId,
     *     EnemyPrefabId,
     *     50  // Pre-allocate 50 clones
     * );
     *
     * poolManager.addPoolConfig(std::move(config));
     * ```
     *
     * @see PrefabId
     * @see PrefabIdComponent
     * @see GameObjectPool
     * @see GameObjectPoolId
     * @see GameObjectPoolManager
     */
    struct GameObjectPoolConfig {

        /**
         * @brief Unique identifier for the pool.
         *
         * Used to reference and retrieve the pool from registries.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId;

        /**
         * @brief Identifier of the prefab template used for cloning.
         *
         * References the prefab GameObject tagged with the matching
         * PrefabIdComponent. The pool system resolves this ID to the
         * actual entity at initialization time.
         */
        const helios::engine::core::data::PrefabId prefabId;

        /**
         * @brief Initial number of objects to pre-allocate in the pool.
         *
         * Determines how many clones of the prefab are created upfront.
         * Higher values reduce runtime allocations but increase memory usage.
         */
        size_t amount;

    };

}