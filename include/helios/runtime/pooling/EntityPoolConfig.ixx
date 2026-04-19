/**
 * @file EntityPoolConfig.ixx
 * @brief Configuration data for EntityPool initialization.
 */
module;

#include <memory>
#include <cstddef>

export module helios.runtime.pooling.EntityPoolConfig;

import helios.runtime.pooling.types.EntityPoolId;
import helios.gameplay.common.types.PrefabId;

export namespace helios::runtime::pooling {

    /**
     * @brief Configuration structure for creating a EntityPool.
     *
     * @details EntityPoolConfig bundles all information required to initialize
     * a EntityPool: a unique pool identifier, a PrefabId referencing the
     * prefab template, and the initial pool size.
     *
     * The PrefabId identifies the prefab Entity that serves as the archetype
     * from which all pooled instances are cloned. The prefab is registered
     * separately via EntityFactory::withPrefabId() and looked up by the
     * pool system at initialization time.
     *
     * Example usage:
     * ```cpp
     * // Create and tag prefab
     * auto enemyPrefab = EntityFactory::instance()
     *     .entity(gameWorld)
     *     .withPrefabId(EnemyPrefabId)
     *     // ... component configuration ...
     *     .make();
     *
     * auto config = std::make_unique<EntityPoolConfig>(
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
     * @see EntityPool
     * @see EntityPoolId
     * @see EntityPoolManager
     */
    struct EntityPoolConfig {

        /**
         * @brief Unique identifier for the pool.
         *
         * Used to reference and retrieve the pool from registries.
         */
        const helios::runtime::pooling::types::EntityPoolId entityPoolId;

        /**
         * @brief Identifier of the prefab template used for cloning.
         *
         * References the prefab Entity tagged with the matching
         * PrefabIdComponent. The pool system resolves this ID to the
         * actual entity at initialization time.
         */
        const helios::gameplay::common::types::PrefabId prefabId;

        /**
         * @brief Initial number of objects to pre-allocate in the pool.
         *
         * Determines how many clones of the prefab are created upfront.
         * Higher values reduce runtime allocations but increase memory usage.
         */
        const size_t amount;

    };

}