/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
 */
module;

#include <format>
#include <memory>
#include <unordered_map>

export module helios.engine.game.GameWorld;

import helios.engine.game.GameObject;
import helios.util.Guid;
import helios.engine.game.UpdateContext;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.scene.Scene;
import helios.engine.game.System;


#define HELIOS_LOG_SCOPE "helios::engine::game::GameWorld"
export namespace helios::engine::game {

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
     * helios::engine::game::GameWorld world;
     *
     * // Add a new entity
     * auto player = std::make_unique<Player>(sceneNode);
     * auto* playerPtr = world.addGameObject(std::move(player));
     *
     * // Lookup by Guid
     * if (auto* obj = world.find(playerPtr->guid())) {
     *     obj->setTranslation({1.0f, 0.0f, 0.0f});
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

    protected:
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

        /**
         * @brief Pointer to the Scene for scene graph integration.
         *
         * @details Non-owning pointer; may be nullptr if scene integration is not required.
         */
        helios::scene::Scene* scene_;

        /**
         * @brief Vector storing all registered Systems.
         *
         * @details Systems are updated in order of registration after all GameObjects
         *          have been updated.
         */
        std::vector<std::unique_ptr<System>> systems_;

    public:

        /**
         * @brief Constructs a GameWorld with a reference to the scene graph.
         *
         * @param scene Pointer to the Scene used for scene graph integration.
         *              May be nullptr if scene graph integration is not required.
         */
        explicit GameWorld(helios::scene::Scene* scene) noexcept : scene_(scene) {}

        /**
         * @brief Returns the scene graph associated with this GameWorld.
         *
         * @return Const pointer to the Scene, or nullptr if not set.
         */
        [[nodiscard]] const helios::scene::Scene* scene() const noexcept;

        /**
         * @brief Adds a System to the GameWorld.
         *
         * @details Creates a System of the specified type with the given constructor arguments,
         *          invokes its `onAdd()` callback, and registers it with this GameWorld.
         *          Systems are updated each frame after GameObjects.
         *
         * @tparam T The System type to add. Must derive from System.
         * @tparam Args Constructor argument types.
         * @param args Arguments forwarded to the System constructor.
         *
         * @return Reference to the newly added System.
         *
         * @note The GameWorld takes ownership of the System.
         */
        template<typename T, typename... Args>
        T& add(Args&&... args) {
            auto system_ptr = std::make_unique<T>(std::forward<Args>(args)...);
            T* raw_ptr = system_ptr.get();

            system_ptr->onAdd(this);
            systems_.push_back(std::move(system_ptr));

            return *raw_ptr;
        }

        /**
         * @brief Retrieves a System of the specified type.
         *
         * @details Searches through all registered Systems using dynamic_cast
         *          to find a matching type.
         *
         * @tparam T The System type to retrieve. Must derive from System.
         *
         * @return Pointer to the System if found, nullptr otherwise.
         *
         * @note Uses dynamic_cast internally; consider type-based registry for
         *       performance-critical paths.
         */
        template<typename T>
        [[nodiscard]] T* get() const {
            for (const auto& system : systems_) {
                if (auto* sys = dynamic_cast<T*>(system.get())) {
                    return sys;
                }
            }
            return nullptr;
        }

        /**
         * @brief Checks if a System of the specified type is registered.
         *
         * @tparam T The System type to check for. Must derive from System.
         *
         * @return True if a System of type T exists, false otherwise.
         */
        template<typename T>
        [[nodiscard]] bool has() const {
            return get<T>() != nullptr;
        }

        /**
         * @brief Updates all GameObjects in the world for the current frame.
         *
         * @param updateContext Context containing deltaTime, input snapshot, game world and command buffer.
         *
         * @note Iterates through all registered GameObjects and invokes their update() method.
         *       The order of updates is not guaranteed due to the underlying unordered_map storage.
         * @note This method is noexcept; individual GameObject::update() implementations should
         *       handle their own exceptions to prevent propagation.
         */
        void update(helios::engine::game::UpdateContext& updateContext) const noexcept;

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
        [[nodiscard]] helios::engine::game::GameObject* addGameObject(std::unique_ptr<GameObject> gameObject);

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
        [[nodiscard]] helios::engine::game::GameObject* find(const helios::util::Guid& guid);

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
        [[nodiscard]] const helios::engine::game::GameObject* find(const helios::util::Guid& guid) const;

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
        [[nodiscard]] std::unique_ptr<helios::engine::game::GameObject> removeGameObject(const GameObject& gameObject);
    };

}