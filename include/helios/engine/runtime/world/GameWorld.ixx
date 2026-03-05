/**
 * @file GameWorld.ixx
 * @brief Central game state container for entities, resources, and the active level.
 */
module;


#include <cassert>
#include <format>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>

export module helios.engine.runtime.world.GameWorld;

import helios.engine.runtime.world.Session;

import helios.engine.runtime.messaging.command.CommandHandler;
import helios.engine.runtime.messaging.command.TypedCommandHandler;

import helios.engine.runtime.world.ResourceRegistry;

import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.Manager;

import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.runtime.world.Level;

import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.EntityManager;
import helios.engine.ecs.EntityRegistry;
import helios.engine.ecs.View;

import helios.engine.core.data;



#define HELIOS_LOG_SCOPE "GameWorld"
export namespace helios::engine::runtime::world {


    /**
     * @brief Central game state container for entities, resources, and the active level.
     *
     * @details
     * GameWorld is the root container for all runtime game state. It owns the
     * EntityRegistry/EntityManager for entity lifecycle, a ResourceRegistry for
     * type-indexed O(1) access to Managers, CommandBuffers, and CommandHandlers,
     * a Session for cross-frame state tracking, and the current Level.
     *
     * ## Key Responsibilities
     *
     * - **Entity Management:** Creates entities via `addGameObject()` returning
     *   lightweight (~16 bytes) `GameObject` wrappers. Entities are identified by
     *   versioned `EntityHandle` for stale reference detection.
     * - **Resource Registry:** Provides `resourceRegistry()` for registering and
     *   looking up Managers, CommandBuffers, and CommandHandlers with O(1) access.
     * - **Component Queries:** Efficient iteration via `view<Components...>()`
     *   with optional `.whereEnabled()` and `.exclude<T>()` filtering.
     * - **Entity Cloning:** Deep-copies entities with all components via `clone()`.
     * - **Manager Coordination:** Initializes, flushes, and resets Managers that
     *   handle cross-cutting concerns (spawning, scoring, pooling).
     * - **Session:** Holds per-run state (tracked game/match states, scores).
     * - **Level Management:** Holds the active Level with arena bounds.
     *
     * ## Usage with GameLoop
     *
     * Systems access the GameWorld indirectly via UpdateContext. Entity queries
     * use views, while mutations are performed through commands submitted via
     * `UpdateContext::queueCommand<T>()`.
     *
     * ```cpp
     * void update(UpdateContext& ctx) noexcept {
     *     for (auto [entity, transform, velocity, active] : ctx.view<
     *         TransformComponent,
     *         VelocityComponent,
     *         Active
     *     >().whereEnabled()) {
     *         // Process matching entities
     *     }
     *
     *     ctx.queueCommand<DespawnCommand>(handle, profileId);
     * }
     * ```
     *
     * ## Resource Registration
     *
     * ```cpp
     * auto& poolMgr  = gameWorld.registerManager<GameObjectPoolManager>();
     * auto& spawnMgr = gameWorld.registerManager<SpawnManager>();
     *
     * gameWorld.init(); // Calls init() on all Managers in registration order
     * ```
     *
     * ## Entity Lifecycle
     *
     * ```cpp
     * auto player = gameWorld.addGameObject();
     * player.add<TransformComponent>(position);
     * player.add<HealthComponent>(100.0f);
     * player.setActive(true);
     *
     * if (auto entity = gameWorld.find(handle)) {
     *     entity->get<HealthComponent>()->takeDamage(10.0f);
     * }
     *
     * auto clone = gameWorld.clone(player);
     * ```
     *
     * @see ResourceRegistry
     * @see GameObject
     * @see EntityHandle
     * @see EntityManager
     * @see View
     * @see UpdateContext
     * @see Manager
     * @see Session
     * @see Level
     */
    class GameWorld {



    protected:


        /**
         * @brief The logger used with this GameWorld instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

        /**
         * @brief The current level loaded in the game world.
         *
         * @details Can be null if no level is currently active.
         */
        std::unique_ptr<Level> level_ = nullptr;


        /**
         * @brief Type-indexed registry for Managers, CommandBuffers, and CommandHandlers.
         *
         * @details Provides O(1) type-based access via ResourceTypeId. Owns all
         * registered Manager and CommandBuffer instances via ErasedUnique.
         */
        ResourceRegistry resourceRegistry_;


        /**
         * @brief Entity registry for handle allocation and validation.
         *
         * @details Manages entity lifecycle including creation, destruction,
         * and stale handle detection via versioning.
         */
        helios::engine::ecs::EntityRegistry entityRegistry_{};

        /**
         * @brief Entity manager for component storage.
         *
         * @details Stores components in type-indexed SparseSets and provides
         * methods for component manipulation. Marked `mutable` to allow const
         * methods to use it without const_cast.
         */
        mutable helios::engine::ecs::EntityManager em_;

        /**
         * @brief The current game session holding state data.
         */
        Session session_;



    public:

        /**
         * @brief Constructs the GameWorld.
         *
         * @details Initializes the EntityManager with the internal EntityRegistry
         * and creates a Session backed by a dedicated GameObject.
         */
        explicit GameWorld() : em_(helios::engine::ecs::EntityManager(entityRegistry_)), session_(Session(addGameObject())) { };

        GameWorld(const GameWorld&) = delete;
        GameWorld operator=(const GameWorld&) = delete;

        GameWorld(const GameWorld&&) = delete;
        GameWorld operator=(const GameWorld&&) = delete;

        /**
         * @brief Returns a reference to the current game session.
         *
         * @return Reference to the Session.
         */
        [[nodiscard]] Session& session() {
            return session_;
        }

        /**
         * @brief Returns a reference to the underlying EntityManager.
         *
         * @return Reference to the EntityManager.
         */
        [[nodiscard]] helios::engine::ecs::EntityManager& entityManager() {
            return em_;
        }

        /**
         * @brief Initializes all registered managers.
         *
         * @details Should be called after all managers have been added and before
         * the game loop starts. Each manager's init() method is invoked with a
         * reference to this GameWorld.
         */
        void init() {
            for (auto& mgr :  resourceRegistry_.managers()) {
                mgr->init(*this);
            }
        }

        /**
         * @brief Sets the current level for the game world.
         *
         * @param level Unique pointer to the Level instance. Ownership is transferred to the GameWorld.
         */
        void setLevel(std::unique_ptr<Level> level) noexcept {
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
         * @return Const pointer to the active Level.
         *
         * @warning Calling this method when hasLevel() returns false results in undefined behavior.
         */
        [[nodiscard]] const Level* level() const noexcept{
            return level_.get();
        }

        /**
         * @brief Checks if a Manager of the specified type is registered.
         *
         * @tparam T The Manager type to check for.
         *
         * @return True if a Manager of type T is registered, false otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<Manager, T>
        [[nodiscard]] bool hasManager() const {
            return resourceRegistry_.has<T>();
        }

        /**
         * @brief Registers a Manager of the specified type.
         *
         * @details Creates a new Manager instance via the ResourceRegistry.
         * The Manager's `init()` is called later during `GameWorld::init()`.
         *
         * @tparam T The Manager type. Must derive from Manager.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the Manager constructor.
         *
         * @return Reference to the newly registered Manager.
         *
         * @pre No Manager of type T is already registered.
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<Manager, T>
        T& registerManager(Args&&... args) {
            assert(!resourceRegistry_.has<T>() && "Manager already registered.");
            return resourceRegistry_.registerResource<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Registers a non-owning CommandHandler reference.
         *
         * @details The handler must outlive the GameWorld (typically guaranteed
         * because the handler is a Manager already owned by the ResourceRegistry).
         *
         * @tparam T The CommandHandler type. Must derive from CommandHandler.
         *
         * @param cmdHandler Reference to the handler.
         *
         * @return Reference to the registered handler.
         *
         * @pre No handler of type T is already registered.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::runtime::messaging::command::CommandHandler, T>
        T& registerCommandHandler(T& cmdHandler) {
            assert(!resourceRegistry_.has<T>() && "CommandHandler already registered.");
            return resourceRegistry_.registerResource<T>(cmdHandler);
        }

        /**
         * @brief Registers an owning resource of arbitrary type.
         *
         * @details Delegates to ResourceRegistry::registerResource. The resource
         * is heap-allocated and owned by the registry.
         *
         * @tparam T The resource type.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the newly created resource.
         */
        template<typename T, typename... Args>
        T& registerResource(Args&&... args) {
            return resourceRegistry_.registerResource<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Returns a reference to a registered Manager.
         *
         * @tparam T The Manager type. Must derive from Manager.
         *
         * @return Reference to the Manager.
         *
         * @pre A Manager of type T must be registered.
         */
        template<typename T>
        requires std::is_base_of_v<Manager, T>
        T& manager() {
            return resourceRegistry_.resource<T>();
        }


        /**
         * @brief Flushes all registered Managers.
         *
         * @details Iterates over all Managers in registration order and invokes
         * `flush(updateContext)` on each. Called by the GameLoop at commit points
         * after the CommandBuffer has been flushed.
         *
         * @param updateContext The current frame's update context.
         */
        void flushManagers(UpdateContext& updateContext) {
            for (auto& mgr : resourceRegistry_.managers()) {
                mgr->flush(updateContext);
            }
        }

        /**
         * @brief Flushes all registered CommandBuffers.
         *
         * @details Iterates over all CommandBuffers in registration order and
         * invokes `flush(*this, updateContext)` on each. Called by the GameLoop
         * at commit points before Managers are flushed.
         *
         * @param updateContext The current frame's update context.
         */
        void flushCommandBuffers(UpdateContext& updateContext) {
            for (auto& buff : resourceRegistry_.commandBuffers()) {
                buff->flush(*this, updateContext);
            }
        }

        /**
         * @brief Creates a new GameObject in the world.
         *
         * @details Allocates an entity handle via the EntityRegistry and returns
         * a lightweight GameObject wrapper. The returned GameObject is ~16 bytes
         * and should be passed by value.
         *
         * @return A new GameObject ready for component attachment.
         *
         * @see GameObject
         * @see EntityManager::create
         */
        [[nodiscard]] helios::engine::ecs::GameObject addGameObject() {
            const auto handle = em_.create();
            return helios::engine::ecs::GameObject(handle, &em_);
        }

        /**
         * @brief Creates a View for iterating entities with specific components.
         *
         * @details Returns a lightweight view that iterates over all entities
         * possessing the specified component types. Use with range-based for loops
         * and structured bindings.
         *
         * ```cpp
         * for (auto [entity, transform, velocity, active] : gameWorld.view<
         *     TransformComponent,
         *     VelocityComponent,
         *     Active
         * >().whereEnabled()) {
         *     // Process matching entities
         * }
         * ```
         *
         * @tparam Components The component types to query for.
         *
         * @return A View for iterating matching entities.
         *
         * @see View
         */
        template <typename... Components>
        [[nodiscard]] auto view() {
            return helios::engine::ecs::View<Components...>(&em_);
        }

        /**
         * @brief Creates a const View for iterating entities with specific components.
         *
         * @tparam Components The component types to query for.
         *
         * @return A const View for iterating matching entities.
         */
        template <typename... Components>
        [[nodiscard]] auto view() const {
            return helios::engine::ecs::View<const Components...>(&em_);
        }

        /**
         * @brief Finds a GameObject by its EntityHandle.
         *
         * @details Validates the handle and returns a GameObject wrapper if valid.
         * Returns std::nullopt if the handle is stale or invalid.
         *
         * @param handle The EntityHandle to look up.
         *
         * @return Optional containing the GameObject if found, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<helios::engine::ecs::GameObject> find(const helios::engine::ecs::EntityHandle handle) {
            if (!em_.isValid(handle)) {
                return std::nullopt;
            }

            return helios::engine::ecs::GameObject(handle, &em_);
        }

        /**
         * @brief Finds a GameObject by its EntityHandle (const version).
         *
         * @param handle The EntityHandle to look up.
         *
         * @return Optional containing the GameObject if found, std::nullopt otherwise.
         */
        [[nodiscard]] std::optional<helios::engine::ecs::GameObject> find(const helios::engine::ecs::EntityHandle handle) const {

            if (!em_.isValid(handle)) {
                return std::nullopt;
            }

            return helios::engine::ecs::GameObject(handle, &em_);
        }

        /**
         * @brief Clones a GameObject and all its components.
         *
         * @details Creates a new entity and copies all components from the source
         * to the target. The new GameObject is initially inactive. Components with
         * `onClone()` hooks will have them invoked after copy construction.
         *
         * @param gameObject The source GameObject to clone.
         *
         * @return A new inactive GameObject with cloned components.
         *
         * @see EntityManager::clone
         */
        [[nodiscard]] helios::engine::ecs::GameObject clone(const helios::engine::ecs::GameObject gameObject) {

            auto newGo = addGameObject();

            newGo.setActive(false);

            em_.clone(gameObject.entityHandle(), newGo.entityHandle());


            return newGo;
        }

        /**
         * @brief Resets all managers and the session to their initial state.
         *
         * @details Called during level transitions or game restarts to clear
         * accumulated state. Invokes reset() on all managers and the session.
         */
        void reset() {

            for (auto& mgr : resourceRegistry_.managers()) {
                mgr->reset();
            }

            session_.reset();

        }

        /**
         * @brief Returns a reference to the ResourceRegistry.
         *
         * @details Use for direct resource registration and lookup. Prefer the
         * convenience methods `registerManager()`, `registerCommandHandler()`,
         * and `manager()` for type-constrained access.
         *
         * @return Reference to the ResourceRegistry.
         */
        ResourceRegistry& resourceRegistry() noexcept {
            return resourceRegistry_;
        }

        const ResourceRegistry& resourceRegistry() const noexcept {
            return resourceRegistry_;
        }


    };

}

