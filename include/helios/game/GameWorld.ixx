/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
 */
module;

#include <format>
#include <memory>
#include <unordered_map>

export module helios.game.GameWorld;

import helios.game.GameObject;
import helios.util.Guid;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::game::GameWorld"
export namespace helios::game {

    /**
     * @brief Central registry and owner of all active GameObjects in the game world.
     *
     * @details The `GameWorld` manages the lifetime and lookup of all `GameObject` instances.
     * It provides fast, `Guid`-based lookup and handles the lifecycle of game
     * entities through a clear ownership model. The world serves as the central registry for all
     * objects and is used by the `CommandBuffer` to execute commands against them.
     *
     * Typical usage:
     * ```cpp
     * helios::game::GameWorld world;
     *
     * // Add a new entity
     * auto player = std::make_unique<Player>(sceneNode);
     * auto* playerPtr = world.addGameObject(std::move(player));
     *
     * // Lookup by Guid
     * if (auto* obj = world.find(playerPtr->guid())) {
     *     obj->translate({1.0f, 0.0f, 0.0f});
     * }
     *
     * // Remove entity (returns ownership)
     * auto removed = world.removeGameObject(*playerPtr);
     * ```
     *
     * @note GameWorld owns all GameObjects added to it. When a GameObject is removed,
     *       ownership is transferred back to the caller via std::unique_ptr.
     */
    class GameWorld {

        /**
         * @brief Hash map storing all active GameObjects, indexed by their Guid.
         *
         * @details Uses std::unordered_map for O(1) average-case lookup performance.
         * The map owns all GameObjects via std::unique_ptr.
         */
        std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>> gameObjects_;

        /**
         * @brief The logger used with this GameWorld instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:

        /**
         * @brief Updates all GameObjects in the world for the current frame.
         *
         * @param deltaTime Time elapsed since the last frame, in seconds.
         *
         * @note Iterates through all registered GameObjects and invokes their update() method.
         *       The order of updates is not guaranteed due to the underlying unordered_map storage.
         * @note This method is noexcept; individual GameObject::update() implementations should
         *       handle their own exceptions to prevent propagation.
         */
        void update(float deltaTime) const noexcept;

        /**
         * @brief Adds a GameObject to the world and transfers ownership.
         *
         * @param gameObject The GameObject to add. Ownership is transferred to the GameWorld.
         *
         * @return Pointer to the added GameObject, or nullptr if addition failed.
         *
         * @retval GameObject* Non-owning pointer to the added object (valid until removed)
         * @retval nullptr If gameObject was null or a GameObject with the same Guid already exists
         *
         * @note The GameWorld takes ownership of the GameObject. The returned pointer remains
         *       valid until the GameObject is removed from the world.
         * @note Attempting to add a nullptr or a GameObject with a duplicate Guid will fail,
         *       return nullptr, and log a warning.
         */
        [[nodiscard]] helios::game::GameObject* addGameObject(std::unique_ptr<GameObject> gameObject);

        /**
         * @brief Finds a GameObject by its unique identifier.
         *
         * @param guid The unique identifier of the GameObject to find.
         *
         * @return Pointer to the GameObject if found, nullptr otherwise.
         *
         * @retval GameObject* Non-owning pointer to the found object
         * @retval nullptr If no GameObject with the specified Guid exists
         *
         * @note This is the non-const overload. Use the const overload for read-only access.
         */
        [[nodiscard]] helios::game::GameObject* find(const helios::util::Guid& guid);

        /**
         * @brief Finds a GameObject by its unique identifier (const overload).
         *
         * @param guid The unique identifier of the GameObject to find.
         *
         * @return Const pointer to the GameObject if found, nullptr otherwise.
         *
         * @retval const GameObject* Non-owning const pointer to the found object
         * @retval nullptr If no GameObject with the specified Guid exists
         *
         * @note This overload is used when the GameWorld is accessed via const reference.
         */
        [[nodiscard]] const helios::game::GameObject* find(const helios::util::Guid& guid) const;

        /**
         * @brief Removes a GameObject from the world and transfers ownership to the caller.
         *
         * @param gameObject Reference to the GameObject to remove.
         *
         * @return Unique pointer to the removed GameObject, or nullptr if not found.
         *
         * @retval std::unique_ptr<GameObject> Ownership of the removed object
         * @retval nullptr If the GameObject was not found in the world
         *
         * @note After successful removal, all non-owning pointers to the GameObject
         *       become invalid unless the caller maintains the returned unique_ptr.
         * @note Attempting to remove a GameObject that doesn't exist returns nullptr
         *       and logs a warning.
         */
        [[nodiscard]] std::unique_ptr<helios::game::GameObject> removeGameObject(const GameObject& gameObject);
    };

}