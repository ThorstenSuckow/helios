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
     * GameObjectPoolManager responsible for this pool config.
     *
     * The prefab serves as the archetype from which all pooled instances are cloned.
     * Components attached to the prefab will be duplicated to each pooled object.
     *
     * Example usage:
     * ```cpp
     * // Create prefab via GameWorld
     * auto enemyPrefab = gameWorld.addGameObject();
     * enemyPrefab.add<RenderableComponent>(mesh, material);
     * enemyPrefab.add<Move2DComponent>();
     * enemyPrefab.add<HealthComponent>(100.0f);
     * enemyPrefab.setActive(false);  // Prefabs should be inactive
     *
     * auto config = std::make_unique<GameObjectPoolConfig>(
     *     GameObjectPoolId{1},
     *     enemyPrefab,
     *     50  // Pre-allocate 50 clones
     * );
     *
     * poolManager.addPoolConfig(std::move(config));
     * ```
     *
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
         * @brief Template object used for cloning pooled instances.
         *
         * @details The prefab defines the component configuration for all objects
         * created by this pool. Since GameObject is lightweight (~16 bytes),
         * it is stored by value. The prefab should be inactive.
         */
        const helios::engine::ecs::GameObject prefab;

        /**
         * @brief Initial number of objects to pre-allocate in the pool.
         *
         * Determines how many clones of the prefab are created upfront.
         * Higher values reduce runtime allocations but increase memory usage.
         */
        const size_t amount;

    };

}