/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
 */
module;

// NOTE: GameObjectView is intentionally a .h header instead of a .ixx module interface.
// Using it as a module interface unit causes Internal Compiler Errors (ICE) in MSVC
// (VS2022/VS2026) when structured bindings are used with the each() iterator.
// The workaround is to include it in the global module fragment.
#include <helios/engine/ecs/query/GameObjectView.h>
#include <memory>
#include <unordered_map>
#include <string>

export module helios.engine.ecs.GameWorld;

import helios.engine.ecs.UpdateContext;
import helios.engine.ecs.GameObject;
import helios.engine.game.Manager;
import helios.engine.game.PoolRequestHandler;
import helios.engine.ecs.Component;
import helios.engine.ecs.CloneableComponent;

import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.game.Level;


import helios.engine.ecs.query.GameObjectFilter;
import helios.engine.core.data.GameObjectPoolId;
import helios.engine.core.data.GameObjectPoolRegistry;


import helios.engine.game.PoolRequestHandlerRegistry;

export namespace helios::engine::core::data {
    class GameObjectPool;
}


#define HELIOS_LOG_SCOPE "helios::engine::ecs::GameWorld"
export namespace helios::engine::ecs {


    /**
     * @brief Central registry for game entities, managers, pools, and the active level.
     *
     * @details
     * The GameWorld is the root container for the game state. It manages the lifecycle
     * of GameObjects, coordinates Managers, and holds the current Level.
     *
     * ## Key Responsibilities
     *
     * - **Entity Management:** Owns all GameObjects and provides amortized O(1) lookup by Guid
     *   via `std::unordered_map`.
     * - **Component Queries:** Efficient iteration over GameObjects with specific components
     *   via `find<Components...>()` with optional filtering by active/enabled state.
     * - **Pool Management:** Registers and provides access to GameObjectPools for entity recycling.
     * - **Manager Coordination:** Holds Managers that handle cross-cutting concerns
     *   (spawning, projectile pooling) and flushes them each frame.
     * - **Level Management:** Holds the active Level instance with arena bounds.
     *
     * ## Usage with GameLoop
     *
     * The GameWorld is passed to Systems via UpdateContext. Systems query GameObjects
     * and their components, while mutations are typically performed through Commands
     * that are flushed by the CommandBuffer.
     *
     * ```cpp
     * // In a System
     * void update(UpdateContext& ctx) noexcept override {
     *     for (auto [obj, move] : ctx.gameWorld().find<Move2DComponent>().each()) {
     *         // Process entities with Move2DComponent
     *     }
     * }
     * ```
     *
     * ## Filtering
     *
     * The `find()` methods accept a `GameObjectFilter` bitmask to control which
     * entities are returned:
     *
     * - `GameObjectFilter::Active` — Only active GameObjects
     * - `GameObjectFilter::ComponentEnabled` — Only objects with enabled components
     *
     * Default filter is `Active | ComponentEnabled`.
     *
     * @see GameObject
     * @see GameObjectFilter
     * @see UpdateContext
     * @see Manager
     * @see Level
     */
    class GameWorld {

        /** @brief Internal map type for GameObject storage. */
        using Map = std::unordered_map<helios::util::Guid, std::unique_ptr<GameObject>>;

        /**
         * @brief Lazy range type for component-filtered GameObject iteration.
         *
         * @details
         * This alias wraps GameObjectView with the appropriate map and component types.
         * The view filters GameObjects on-the-fly during iteration, yielding only
         * those that possess all specified component types.
         *
         * @note GameObjectView is included as a .h header in the global module fragment
         *       due to MSVC ICE issues when used as a .ixx module interface unit.
         *
         * @tparam MapT The map type (const or non-const).
         * @tparam Cs The component types to filter by.
         */
        template<class MapT, class... Cs>
        using GameObjectRange = GameObjectView<MapT, helios::engine::ecs::GameObject, Cs...>;


    protected:
        /**
         * @brief Hash map storing all active GameObjects, indexed by their Guid.
         *
         * @details Uses `std::unordered_map` for amortized O(1) average-case lookup.
         * The map owns all GameObjects via `std::unique_ptr`. Worst-case lookup is O(n)
         * in case of hash collisions, but the sequential Guid generation ensures good
         * hash distribution in practice.
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
         * @brief Collection of registered Manager instances.
         *
         * @details Managers handle cross-cutting concerns such as object pooling,
         * spawn management, and projectile lifecycle. They are initialized via init()
         * and flushed each frame via flushManagers().
         */
        std::vector<std::unique_ptr<helios::engine::game::Manager>> managers_;

        /**
         * @brief The current level loaded in the game world.
         *
         * @details Can be null if no level is currently active.
         */
        std::unique_ptr<helios::engine::game::Level> level_ = nullptr;

        /**
         * @brief Registry of GameObjectPools for entity recycling.
         *
         * @details Pools enable efficient reuse of GameObjects without repeated
         * allocation/deallocation. Each pool is identified by a GameObjectPoolId.
         */
        helios::engine::core::data::GameObjectPoolRegistry pools_{};

        /**
         * @brief Registry mapping pool IDs to their request handlers.
         *
         * @details Request handlers process spawn/despawn requests for specific pools,
         * enabling custom lifecycle management per pool type.
         */
        helios::engine::game::PoolManagerRegistry poolManagerRegistry_{};



    public:

        /**
         * @brief Retrieves a GameObjectPool by its identifier.
         *
         * @param gamePoolId The unique identifier of the pool.
         *
         * @return Pointer to the pool if found, nullptr otherwise.
         */
        [[nodiscard]] helios::engine::core::data::GameObjectPool* pool(
            helios::engine::core::data::GameObjectPoolId gamePoolId) const {
            return pools_.pool(gamePoolId);
        }

        /**
         * @brief Registers a new GameObjectPool with the world.
         *
         * @param gamePoolId The unique identifier for the pool.
         * @param gameObjectPool The pool instance. Ownership is transferred.
         *
         * @return Pointer to the added pool, or nullptr on failure.
         */
        [[nodiscard]] helios::engine::core::data::GameObjectPool* addPool(
            helios::engine::core::data::GameObjectPoolId gamePoolId,
            std::unique_ptr<helios::engine::core::data::GameObjectPool> gameObjectPool) {
            auto* pool =  pools_.addPool(gamePoolId, std::move(gameObjectPool));
            assert(pool != nullptr && "unexpected nullptr for pool");
            if (pool == nullptr) {
                return nullptr;
            }
            return pool;
        }

        /**
         * @brief Initializes all registered managers.
         *
         * @details Should be called after all managers have been added and before
         * the game loop starts. Each manager's init() method is invoked with a
         * reference to this GameWorld.
         */
        void init() {
            for (auto& mgr :  managers_) {
                mgr->init(*this);
            }
        }

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
         * @brief Checks if a Manager of the specified type is registered.
         *
         * @tparam T The Manager type to check for.
         *
         * @return True if a Manager of type T is registered, false otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::game::Manager, T>
        [[nodiscard]] bool hasManager() const {
            return getManager<T>() != nullptr;
        }

        /**
         * @brief Adds and registers a Manager of the specified type.
         *
         * @details Creates a new Manager instance and adds it to the world.
         * The manager's onAdd() callback is invoked after registration.
         *
         * @tparam T The Manager type to add. Must derive from Manager.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the Manager constructor.
         *
         * @return Reference to the newly added Manager.
         *
         * @pre No Manager of type T is already registered.
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<helios::engine::game::Manager, T>
        T& addManager(Args&&... args) {

            assert(!hasManager<T>() && "Manager already registered.");

            auto manager = std::make_unique<T>(std::forward<Args>(args)...);
            auto* manager_ptr = manager.get();

            managers_.push_back(std::move(manager));

            manager_ptr->onAdd(*this);
            return *manager_ptr;
        }

        /**
         * @brief Registers a PoolRequestHandler for a specific pool.
         *
         * @details Associates a handler with a pool ID. The handler processes
         * spawn and despawn requests for entities in that pool.
         *
         * @param gameObjectPoolId The pool identifier to associate with the handler.
         * @param poolManager Reference to the handler to register.
         *
         * @return True if registration succeeded, false if already registered.
         */
        bool registerPoolRequestHandler(
            const helios::engine::core::data::GameObjectPoolId gameObjectPoolId,
            helios::engine::game::PoolRequestHandler& poolManager
        ) {
            bool added = poolManagerRegistry_.add(gameObjectPoolId, poolManager);

            assert(added && "PoolManager already registered");

            return added;
        }

        /**
         * @brief Retrieves the PoolRequestHandler for a specific pool.
         *
         * @param gameObjectPoolId The pool identifier.
         *
         * @return Pointer to the handler if found, nullptr otherwise.
         */
        helios::engine::game::PoolRequestHandler* poolRequestHandler(helios::engine::core::data::GameObjectPoolId gameObjectPoolId) {
            return poolManagerRegistry_.get(gameObjectPoolId);
        }

        /**
         * @brief Retrieves a registered Manager by type.
         *
         * @tparam T The Manager type to retrieve. Must derive from Manager.
         *
         * @return Pointer to the Manager if found, nullptr otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::game::Manager, T>
        [[nodiscard]] T* getManager() const {

            for (auto& mgr : managers_) {
                if (auto* c = dynamic_cast<T*>(mgr.get())) {
                    return c;
                }
            }

            return nullptr;
        }

        /**
         * @brief Flushes all registered managers.
         *
         * @details Called during the game loop to allow managers to process
         * their queued requests (e.g., spawn requests, pool returns).
         *
         * @param updateContext The current frame's update context.
         */
        void flushManagers(helios::engine::ecs::UpdateContext& updateContext) {
            for (auto& mgr :  managers_) {
                mgr->flush(*this, updateContext);
            }
        }



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
        [[nodiscard]] helios::engine::ecs::GameObject* addGameObject(std::unique_ptr<GameObject> gameObject);

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
        [[nodiscard]] helios::engine::ecs::GameObject* find(const helios::util::Guid& guid);

        /**
         * @brief Finds all GameObjects that have the specified component types.
         *
         * @details
         * Returns a lazy range that filters GameObjects on-the-fly during iteration.
         * Only GameObjects possessing all specified component types are yielded.
         *
         * The query is controlled by a `GameObjectFilter` bitmask:
         *
         * | Filter | Effect |
         * |--------|--------|
         * | `Active` | Only `obj->isActive() == true` |
         * | `Inactive` | Only `obj->isActive() == false` |
         * | `ComponentEnabled` | Only objects where queried components are enabled |
         * | `ComponentDisabled` | Only objects where queried components are disabled |
         *
         * Filters can be combined: `Active | ComponentEnabled` (default).
         *
         * Example usage:
         * ```cpp
         * // Range-based for loop with default filter (Active + ComponentEnabled)
         * for (auto* obj : gameWorld.find<Move2DComponent, SceneNodeComponent>()) {
         *     auto* move = obj->get<Move2DComponent>();
         *     move->setVelocity({1.0f, 0.0f, 0.0f});
         * }
         *
         * // Using each() for direct component access (structured bindings)
         * for (auto [obj, move] : gameWorld.find<Move2DComponent>().each()) {
         *     move.get().setVelocity({1.0f, 0.0f, 0.0f});
         * }
         *
         * // Custom filter: find inactive objects
         * for (auto* obj : gameWorld.find<HealthComponent>(GameObjectFilter::Inactive)) {
         *     // Process inactive entities (e.g., respawn logic)
         * }
         * ```
         *
         * @tparam Cs The component types to filter by. GameObjects must have all types.
         *
         * @param query Filter bitmask controlling which entities are included.
         *              Default: `Active | ComponentEnabled`.
         *
         * @return A GameObjectView object that can be iterated over.
         *
         * @see GameObjectFilter
         * @see GameObjectView
         */
        template<class... Cs>
        [[nodiscard]] auto find(GameObjectFilter query = GameObjectFilter::Active | GameObjectFilter::ComponentEnabled) {
            return GameObjectRange<Map, Cs...>(gameObjects_, query);
        }

        /**
         * @brief Finds all GameObjects that have the specified component types (const overload).
         *
         * @details
         * Const version of the component-based find. Returns const pointers to GameObjects.
         * See the non-const overload for detailed usage and filter documentation.
         *
         * @tparam Cs The component types to filter by. GameObjects must have all types.
         *
         * @param query Filter bitmask controlling which entities are included.
         *              Default: `Active | ComponentEnabled`.
         *
         * @return A const GameObjectView object that can be iterated over.
         *
         * @see GameObjectFilter
         */
        template<class... Cs>
        [[nodiscard]] auto find(GameObjectFilter query = GameObjectFilter::Active | GameObjectFilter::ComponentEnabled) const {
            return GameObjectRange<const Map, Cs...>(gameObjects_, query);
        }

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
        [[nodiscard]] const helios::engine::ecs::GameObject* find(const helios::util::Guid& guid) const;

        /**
         * @brief Creates a clone of an existing GameObject.
         *
         * @details Clones all Cloneable components from the source GameObject.
         * The cloned GameObject is initially inactive and receives a new Guid.
         * Non-cloneable components are skipped with a warning.
         *
         * @param gameObject The source GameObject to clone.
         *
         * @return Pointer to the newly created clone, or nullptr on failure.
         *
         * @note The clone is added to the world in an inactive state.
         */
        [[nodiscard]] helios::engine::ecs::GameObject* clone(helios::engine::ecs::GameObject& gameObject) {


            auto newGo = std::make_unique<helios::engine::ecs::GameObject>();
            /**
             * @todo Optional ClonePolicy where rules are specified?
             */
            newGo->setActive(false);

            for (const auto& component : gameObject.components()) {
                if (const auto* cc = dynamic_cast<const Cloneable*>(component.get())) {
                    auto cComponent = cc->clone();
                    // use getOrAdd since cloned components may have already added
                    // other components in onAttach(), which we will not defer for now.
                    // we assume that the cloned component that allows for adding
                    // components in onAttach is the source of truth for said components
                    newGo->getOrAdd(std::move(cComponent));
                } else {
                    logger_.warn("Component not cloneable.");
                }

            }

            return addGameObject(std::move(newGo));
        }


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
        [[nodiscard]] std::unique_ptr<helios::engine::ecs::GameObject> removeGameObject(const GameObject& gameObject);

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

