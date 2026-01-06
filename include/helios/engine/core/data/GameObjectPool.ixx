/**
 * @file GameObjectPool.ixx
 * @brief Object pool for efficient GameObject management and recycling.
 *
 * Provides a memory-efficient pooling mechanism for frequently spawned and despawned
 * GameObjects, such as projectiles, particles, or enemies. Pre-allocates objects at
 * construction time and reuses them to avoid runtime allocation overhead.
 */
module;

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>


export module helios.engine.core.data.GameObjectPool;

import helios.engine.game.System;

import helios.scene.SceneNode;

import helios.engine.game.rendering.components.RenderableComponent;
import helios.engine.game.scene.components.SceneNodeComponent;
import helios.engine.game.spatial.transform.components.TransformComponent;
import helios.engine.game.physics.motion.components.Move2DComponent;

import helios.engine.game.GameObject;
import helios.engine.game.GameWorld;
import helios.engine.game.UpdateContext;

import helios.util.Guid;

export namespace helios::engine::game {

    /**
     * @brief A reusable object pool for GameObjects.
     *
     * @details
     * GameObjectPool implements the Object Pool pattern to efficiently manage
     * GameObjects that are frequently created and destroyed. Instead of allocating
     * new objects at runtime, the pool pre-allocates a fixed number of objects
     * during warmup and recycles them.
     *
     * The pool tracks objects by their Guid and validates them against the GameWorld
     * on acquire to handle external removal gracefully.
     *
     * Typical use cases:
     * - Projectile/bullet spawning
     * - Particle systems
     * - Enemy wave spawning
     *
     * Example usage:
     * ```cpp
     * auto factory = []() {
     *     auto bullet = std::make_unique<GameObject>();
     *     bullet->addComponent<Move2DComponent>();
     *     return bullet;
     * };
     *
     * GameObjectPool bulletPool(gameWorld, factory, 100);
     *
     * // Acquire an object from the pool
     * if (auto* bullet = bulletPool.acquire()) {
     *     bullet->get<Move2DComponent>()->setVelocity({0, 1, 0});
     * }
     *
     * // Release the object back to the pool
     * bulletPool.release(bullet);
     * ```
     */
    class GameObjectPool {

        /**
         * @brief Factory function type for creating new GameObjects.
         *
         * The factory is invoked during pool warmup to create all pooled objects.
         * It should return a fully configured GameObject ready for use.
         */
        using Factory = std::function<std::unique_ptr<helios::engine::game::GameObject>()>;

    protected:

        /**
         * @brief Maps active GameObject Guids to their index in activeGameObjects_.
         *
         * Enables O(1) lookup for release operations.
         */
        std::unordered_map<helios::util::Guid, size_t> activeIndex_;

        /**
         * @brief List of Guids for currently active (in-use) GameObjects.
         */
        std::vector<helios::util::Guid> activeGameObjects_;

        /**
         * @brief List of Guids for currently inactive (available) GameObjects.
         */
        std::vector<helios::util::Guid> inactiveGameObjects_;

        /**
         * @brief The maximum number of objects this pool manages.
         */
        size_t poolSize_ = 0;

        /**
         * @brief Reference to the GameWorld that owns the pooled GameObjects.
         */
        helios::engine::game::GameWorld& gameWorld_;

        /**
         * @brief Factory function used to create new GameObjects during warmup.
         */
        Factory gameObjectFactory_;

        /**
         * @brief Unique identifier for this pool instance.
         */
        const helios::util::Guid guid_;

        /**
         * @brief Pre-allocates all pooled objects and adds them to the GameWorld.
         *
         * @details
         * Creates poolSize_ GameObjects using the factory, sets them inactive,
         * adds them to the GameWorld, and tracks their Guids for later acquisition.
         * Called automatically during construction.
         */
        void warmup() {
            for (unsigned int i = 0; i < poolSize_; i++) {
                std::unique_ptr<helios::engine::game::GameObject> gameObject = gameObjectFactory_();
                gameObject->setActive(false);
                helios::util::Guid guid = gameObject->guid();
                std::ignore = gameWorld_.addGameObject(std::move(gameObject));
                inactiveGameObjects_.push_back(guid);
            }
        }

    public:

        /**
         * @brief Constructs a new GameObjectPool.
         *
         * @param gameWorld Reference to the GameWorld that will own the pooled objects.
         * @param gameObjectFactory Factory function to create new GameObjects.
         * @param poolSize Number of objects to pre-allocate in the pool.
         *
         * @note The pool immediately calls warmup() to pre-allocate all objects.
         */
        explicit GameObjectPool(
            helios::engine::game::GameWorld& gameWorld,
            Factory gameObjectFactory,
            size_t poolSize
        ) :
        poolSize_(poolSize),
        gameWorld_(gameWorld),
        gameObjectFactory_(std::move(gameObjectFactory)),
        guid_(helios::util::Guid::generate()) {

            activeGameObjects_.reserve(poolSize);
            inactiveGameObjects_.reserve(poolSize);

            warmup();
        }

        /**
         * @brief Returns the unique identifier of this pool.
         *
         * @return The Guid assigned to this pool instance.
         */
        [[nodiscard]] helios::util::Guid guid() const noexcept {
            return guid_;
        }


        /**
         * @brief Acquires the next available GameObject from the pool.
         *
         * @details
         * Retrieves an inactive GameObject, validates it against the GameWorld,
         * marks it as active, and returns it. If a Guid references an object
         * that no longer exists in the GameWorld (e.g., externally removed),
         * it is silently discarded and the next available object is tried.
         *
         * @return Pointer to an activated GameObject, or nullptr if the pool is exhausted.
         *
         * @note The returned object is marked as active and tracked internally.
         *       Call release() to return it to the pool when done.
         */
        [[nodiscard]] helios::engine::game::GameObject* acquire() {

            if (inactiveGameObjects_.empty()) {
                return nullptr;
            }

            while (!inactiveGameObjects_.empty()) {

                auto guid = inactiveGameObjects_.back();
                auto* gameObject = gameWorld_.find(guid);

                inactiveGameObjects_.pop_back();

                if (gameObject == nullptr) {
                    continue;
                }

                activeIndex_[guid] = activeGameObjects_.size();
                activeGameObjects_.push_back(guid);
                gameObject->setActive(true);

                return gameObject;
            }

            return nullptr;

        }

        /**
         * @brief Releases a GameObject back to the pool.
         *
         * @param gameObject Pointer to the GameObject to release. May be nullptr.
         *
         * @return True if the object was successfully released, false if gameObject
         *         was nullptr or not tracked by this pool.
         */
        bool release(const helios::engine::game::GameObject* gameObject) {
            return gameObject != nullptr ? release(gameObject->guid()) : false;
        }

        /**
         * @brief Releases a GameObject back to the pool by its Guid.
         *
         * @details
         * Validates the Guid against both the GameWorld and the active tracking list.
         * Uses swap-and-pop for O(1) removal from the active list. The object is
         * marked inactive and added to the inactive list for future acquisition.
         *
         * @param guid The unique identifier of the GameObject to release.
         *
         * @return True if the object was successfully released, false if the Guid
         *         was not found in the GameWorld or not tracked as active.
         */
        bool release(const helios::util::Guid& guid) {

            auto* gameObject = gameWorld_.find(guid);
            auto it = activeIndex_.find(guid);

            if (!gameObject || it == activeIndex_.end()) {
                return false;
            }


            size_t idx = it->second;
            auto lastGuid = activeGameObjects_.back();

            // swap the last guid in activeGameObjects with the
            // guid to remove, effectively overwriting guid
            // to release with a currently active guid
            activeGameObjects_[idx] = lastGuid;
            activeIndex_[lastGuid] = idx;

            // the swap operation has create a duplicate entry,
            // remove the one at the tail
            activeGameObjects_.pop_back();

            // clear the queried guid from active index and update
            // inactiveGameObjects
            activeIndex_.erase(it);
            inactiveGameObjects_.push_back(guid);

            gameObject->setActive(false);

            return true;
        }

        /**
         * @brief Retrieves all currently inactive GameObjects.
         *
         * @details
         * Iterates the inactive Guid list and resolves each to a GameObject pointer.
         * Objects that no longer exist in the GameWorld are silently skipped.
         *
         * @return Vector of pointers to inactive GameObjects. The vector may be
         *         smaller than the inactive count if some objects were externally removed.
         *
         * @note This operation has O(n) complexity where n is the number of inactive objects.
         */
        std::vector<helios::engine::game::GameObject*> inactiveGameObjects() {

            std::vector<helios::engine::game::GameObject*> gameObjects{};

            for (int i = inactiveGameObjects_.size() - 1; i >= 0; i--) {

                auto guid = inactiveGameObjects_[i];
                auto* gameObject = gameWorld_.find(guid);

                if (gameObject == nullptr) {
                    continue;
                }

                gameObjects.push_back(gameObject);
            }

            return gameObjects;

        }

        /**
         * @brief Returns the number of active game objects.
         *
         * @return The number of active game objects.
         */
        [[nodiscard]] size_t activeCount() const noexcept {
            return activeGameObjects_.size();
        }

        /**
         * @brief Returns the number of inactive game objects.
         *
         * @return The number of inactive game objects.
         */
        [[nodiscard]] size_t inactiveCount() const noexcept {
            return inactiveGameObjects_.size();
        }
    };

}
