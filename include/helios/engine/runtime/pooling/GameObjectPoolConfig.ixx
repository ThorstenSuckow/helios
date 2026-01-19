/**
 * @file GameObjectPoolConfig.ixx
 * @brief Configuration data for GameObjectPool initialization.
 */
module;

#include <memory>

export module helios.engine.runtime.pooling.GameObjectPoolConfig;

import helios.engine.ecs.GameObject;
import helios.engine.core.data.GameObjectPoolId;

export namespace helios::engine::runtime::pooling {

    /**
     * @brief Configuration structure for creating a GameObjectPool.
     *
     * @details GameObjectPoolConfig bundles all information required to initialize
     * a GameObjectPool: a unique identifier, a prefab template for cloning, and
     * the initial pool size. This configuration is typically passed to the
     * GameObjectPoolManager responsibkle for this pool config.
     *
     * The prefab serves as the archetype from which all pooled instances are cloned.
     * Components attached to the prefab will be duplicated to each pooled object.
     *
     * Example usage:
     * ```cpp
     * auto enemyPrefab = std::make_unique<GameObject>();
     * enemyPrefab->add<RenderableComponent>(mesh, material);
     * enemyPrefab->add<Move2DComponent>();
     *
     * GameObjectPoolConfig config{
     *     .gameObjectPoolId = GameObjectPoolId{1},
     *     .prefab = std::move(enemyPrefab),
     *     .amount = 50
     * };
     *
     * poolManager.addPoolConfig(std::move(config));
     * ```
     *
     * @see GameObjectPool
     * @see GameObjectPoolId
     * @see GameObjectPoolRegistry
     */
    struct GameObjectPoolConfig {

        /**
         * @brief Unique identifier for the pool.
         *
         * Used to reference and retrieve the pool from registries.
         */
        const helios::engine::core::data::GameObjectPoolId gameObjectPoolId;

        /**
         * @brief Template object used for cloning pooled instances.
         *
         * The prefab defines the component configuration for all objects
         * created by this pool. Ownership is transferred to the pool.
         */
        const std::unique_ptr<helios::engine::ecs::GameObject> prefab;

        /**
         * @brief Initial number of objects to pre-allocate in the pool.
         *
         * Determines how many clones of the prefab are created upfront.
         * Higher values reduce runtime allocations but increase memory usage.
         */
        const size_t amount;

    };

}