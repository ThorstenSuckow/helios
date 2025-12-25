/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
 */
module;

#include <memory>
#include <unordered_map>

export module helios.engine.game.GameWorld;

import helios.engine.game.GameObject;
import helios.util.Guid;
import helios.engine.game.UpdateContext;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.game.System;
import helios.engine.game.Level;

#define HELIOS_LOG_SCOPE "helios::engine::game::GameWorld"
export namespace helios::engine::game {


    /**
     * @brief Central registry for managing game entities, systems, and the active level.
     *
     * @details
     * The GameWorld is the root container for the game state. It manages the lifecycle
     * of GameObjects, executes Systems, and holds the current Level.
     *
     * Key responsibilities:
     * - **Entity Management:** Owns all GameObjects and provides lookup by Guid.
     * - **System Execution:** Manages and updates registered Systems.
     * - **Level Management:** Holds the active Level instance.
     * - **Update Loop:** Orchestrates the frame update by updating GameObjects and then Systems.
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
         * @brief Vector storing all registered Systems.
         *
         * @details Systems are updated in order of registration after all GameObjects
         *          have been updated.
         */
        std::vector<std::unique_ptr<System>> systems_;

        /**
         * @brief The current level loaded in the game world.
         *
         * @details Can be null if no level is currently active.
         */
        std::unique_ptr<helios::engine::game::Level> level_ = nullptr;

    public:


        explicit GameWorld() = default;


        /**
         * @brief Sets the current level for the game world.
         *
         * @param level Unique pointer to the Level instance. Ownership is transferred to the GameWorld.
         */
        void setLevel(std::unique_ptr<helios::engine::game::Level> level) noexcept {
            level_ = std::move(level);
        }

        /**
         * @brief Checks if a level is currently loaded.
         *
         * @return True if a level is set, false otherwise.
         */
        [[nodiscard]] bool hasLevel() const noexcept{
            return level_ != nullptr;
        }

        /**
         * @brief Retrieves the currently loaded level.
         *
         * @return Reference to the active Level.
         *
         * @warning Calling this method when hasLevel() returns false results in undefined behavior.
         */
        [[nodiscard]] const helios::engine::game::Level& level() const noexcept{
            return *level_;
        }

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
         * @brief Updates the game world state for the current frame.
         *
         * @details
         * This method performs the following steps:
         * 1. Updates all active GameObjects (invoking their `update()` method).
         * 2. Updates all registered Systems (invoking their `update()` method).
         *
         * @param updateContext Context containing deltaTime, input snapshot, game world and command buffer.
         *
         * @note The order of GameObject updates is not guaranteed. Systems are updated in registration order.
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

        /**
         * @brief Retrieves a const ref to the map of all active GameObjects.
         *
         * @return A const reference to the internal map of GameObjects, indexed by Guid.
         *
         * @warning Modifying the map directly (e.g. adding/removing elements) bypasses
         *          GameWorld's management logic and should be avoided. Use addGameObject()
         *          and removeGameObject() instead.
         */
        [[nodiscard]] const std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>>& gameObjects() const noexcept {
            return gameObjects_;
        }
    };

}

