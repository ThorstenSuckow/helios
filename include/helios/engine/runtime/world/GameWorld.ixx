/**
 * @file GameWorld.ixx
 * @brief Central registry for managing game entities.
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

import helios.engine.runtime.world.UpdateContext;
import helios.engine.ecs.GameObject;
import helios.engine.runtime.world.Manager;
import helios.engine.runtime.spawn.SpawnCommandHandler;

import helios.engine.modules.ui.UiActionCommandHandler;

import helios.engine.mechanics.scoring.ScoreCommandHandler;
import helios.engine.state.StateCommandHandler;
import helios.engine.state.TypedStateCommandHandler;

import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.engine.runtime.world.Level;

import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.EntityManager;
import helios.engine.ecs.EntityRegistry;
import helios.engine.ecs.View;

import helios.engine.core.data;

import helios.engine.runtime.spawn.SpawnCommandHandlerRegistry;



#define HELIOS_LOG_SCOPE "helios::engine::runtime::world::GameWorld"
export namespace helios::engine::runtime::world {


    /**
     * @brief Central registry for game entities, managers, pools, and the active level.
     *
     * @details
     * The GameWorld is the root container for the game state. It manages the lifecycle
     * of entities via EntityRegistry/EntityManager, coordinates Managers, and holds
     * the current Level.
     *
     * ## Key Responsibilities
     *
     * - **Entity Management:** Creates entities via `addGameObject()` which returns
     *   lightweight (~16 bytes) `GameObject` wrappers. Entities are identified by
     *   versioned `EntityHandle` for stale reference detection.
     * - **Component Queries:** Efficient iteration over entities with specific components
     *   via `view<Components...>()` with optional filtering via `.whereEnabled()` and
     *   `.exclude<T>()`.
     * - **Entity Cloning:** Deep-copy entities with all components via `clone()`.
     * - **Pool Management:** Registers and provides access to SpawnCommandHandlers for
     *   entity recycling.
     * - **Manager Coordination:** Holds Managers that handle cross-cutting concerns
     *   (spawning, projectile pooling) and flushes them each frame.
     * - **Level Management:** Holds the active Level instance with arena bounds.
     *
     * ## Usage with GameLoop
     *
     * The GameWorld is passed to Systems via UpdateContext. Systems query entities
     * using views, while mutations are typically performed through Commands
     * that are flushed by the CommandBuffer.
     *
     * ```cpp
     * // In a System
     * void update(UpdateContext& ctx) noexcept override {
     *     for (auto [entity, transform, velocity, active] : gameWorld_->view<
     *         TransformComponent,
     *         VelocityComponent,
     *         Active
     *     >().whereEnabled()) {
     *         // Process entities with both components
     *     }
     * }
     * ```
     *
     * ## Entity Lifecycle
     *
     * ```cpp
     * // Create entity
     * auto player = gameWorld.addGameObject();
     *
     * // Add components
     * player.add<TransformComponent>(position);
     * player.add<HealthComponent>(100.0f);
     *
     * // Activate
     * player.setActive(true);
     *
     * // Find by handle
     * if (auto entity = gameWorld.find(handle)) {
     *     entity->get<HealthComponent>()->takeDamage(10.0f);
     * }
     *
     * // Clone
     * auto clone = gameWorld.clone(player);
     * ```
     *
     * @see GameObject
     * @see EntityHandle
     * @see EntityManager
     * @see View
     * @see UpdateContext
     * @see Manager
     * @see Level
     */
    class GameWorld {



    protected:


        /**
         * @brief Hash map storing all active GameObjects, indexed by their Guid.
         *
         * @details Uses `std::unordered_map` for amortized O(1) average-case lookup.
         * The map owns all GameObjects via `std::unique_ptr`. Worst-case lookup is O(n)
         * in case of hash collisions, but the sequential Guid generation ensures good
         * hash distribution in practice.
         */
        std::unordered_map<helios::util::Guid, std::unique_ptr<helios::engine::ecs::GameObject>> gameObjects_;


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
        std::vector<std::unique_ptr<helios::engine::runtime::world::Manager>> managers_;

        /**
         * @brief The current level loaded in the game world.
         *
         * @details Can be null if no level is currently active.
         */
        std::unique_ptr<helios::engine::runtime::world::Level> level_ = nullptr;

        /**
         * @brief Registered handler for score commands.
         *
         * @details Receives score update commands and processes them
         * according to the scoring system logic.
         */
        helios::engine::mechanics::scoring::ScoreCommandHandler* scoreCommandHandler_ = nullptr;

        /**
         * @brief Registered handlers for state commands, indexed by StateTypeId.
         *
         * @details Stores pointers to TypedStateCommandHandler instances. Each
         * state type (GameState, MatchState, etc.) has a unique index computed
         * from StateTypeId.
         */
        std::vector<helios::engine::state::StateCommandHandler*> stateCommandHandlers_;

        /**
         * @brief Registered handler for UI action commands.
         */
        helios::engine::modules::ui::UiActionCommandHandler* uiActionCommandHandler_ = nullptr;

        /**
         * @brief Registry for mapping spawn profiles to their command handlers.
         *
         * @details
         * Stores the association between SpawnProfileIds and their corresponding
         * SpawnCommandHandlers. This allows the system to look up the correct handler
         * (e.g., a specific object pool) when processing spawn commands.
         */
        helios::engine::runtime::spawn::SpawnCommandHandlerRegistry spawnCommandHandlerRegistry_{};

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
         * @brief Central registry for game entities, managers, pools, and the active level.
         *
         * @details
         * The GameWorld serves as the primary container for managing the game state and its subsystems.
         * It facilitates the lifecycle of entities, oversees component-based management, coordinates Managers,
         * and holds a reference to the active Level instance.
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
        void setLevel(std::unique_ptr<helios::engine::runtime::world::Level> level) noexcept {
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
        [[nodiscard]] const helios::engine::runtime::world::Level* level() const noexcept{
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
        requires std::is_base_of_v<helios::engine::runtime::world::Manager, T>
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
        requires std::is_base_of_v<helios::engine::runtime::world::Manager, T>
        T& addManager(Args&&... args) {

            assert(!hasManager<T>() && "Manager already registered.");

            auto manager = std::make_unique<T>(std::forward<Args>(args)...);
            auto* manager_ptr = manager.get();

            managers_.push_back(std::move(manager));

            manager_ptr->onAdd(*this);
            return *manager_ptr;
        }

        /**
         * @brief Registers a SpawnCommandHandler for a specific spawn profile.
         *
         * @details Associates a handler with a spawn profile ID. The handler processes
         * spawn and despawn requests for entities associated with that profile.
         *
         * @param spawnProfileId The spawn profile identifier to associate with the handler.
         * @param poolManager Reference to the handler to register.
         *
         * @return True if registration succeeded, false if already registered.
         */
        bool registerSpawnCommandHandler(
            const helios::engine::core::data::SpawnProfileId spawnProfileId,
            helios::engine::runtime::spawn::SpawnCommandHandler& poolManager
        ) {
            bool added = spawnCommandHandlerRegistry_.add(spawnProfileId, poolManager);

            assert(added && "PoolManager already registered");

            return added;
        }

        /**
         * @brief Registers a handler for UI action commands.
         *
         * @param uiActionCommandHandler Reference to the handler to register.
         *
         * @return True if registration succeeded.
         */
        bool registerUiActionCommandHandler(
            helios::engine::modules::ui::UiActionCommandHandler& uiActionCommandHandler
        ) {
            assert(!uiActionCommandHandler_ && "uiActionCommandHandler already registered");

            uiActionCommandHandler_ = &uiActionCommandHandler;

            return true;
        }

        /**
         * @brief Retrieves the registered UI action command handler.
         *
         * @return Pointer to the handler, or nullptr if not registered.
         */
        [[nodiscard]] helios::engine::modules::ui::UiActionCommandHandler* uiActionCommandHandler() {
            return uiActionCommandHandler_;
        }

        /**
         * @brief Retrieves a SpawnCommandHandler for a specific spawn profile.
         *
         * @details Used to submit spawn/despawn commands to the handler responsible
         * for a particular spawn profile (e.g., bullet pool, enemy pool).
         *
         * @param spawnProfileId The spawn profile identifier to look up.
         *
         * @return Pointer to the handler, or nullptr if not registered.
         *
         * @see registerSpawnCommandHandler()
         * @see SpawnCommandHandler
         */
        [[nodiscard]] helios::engine::runtime::spawn::SpawnCommandHandler* spawnCommandHandler(
            const helios::engine::core::data::SpawnProfileId spawnProfileId) {
            return spawnCommandHandlerRegistry_.get(spawnProfileId);
        }

        /**
         * @brief Registers a handler for score commands.
         *
         * @details Associates a ScoreCommandHandler with this GameWorld.
         * Only one handler can be registered at a time.
         *
         * @param scoreCommandHandler Reference to the handler to register.
         *
         * @return True if registration succeeded, false if already registered.
         */
        bool registerScoreCommandHandler(
            helios::engine::mechanics::scoring::ScoreCommandHandler& scoreCommandHandler
        ) {
            assert(!scoreCommandHandler_ && "ScoreCommandHandler already registered");

            scoreCommandHandler_ = &scoreCommandHandler;

            return true;
        }

        /**
         * @brief Retrieves the registered ScoreCommandHandler.
         *
         * @return Pointer to the handler, or nullptr if not registered.
         */
        [[nodiscard]] helios::engine::mechanics::scoring::ScoreCommandHandler* scoreCommandHandler() {
            return scoreCommandHandler_;
        }

        /**
         * @brief Registers a handler for state commands of a specific type.
         *
         * @details Associates a TypedStateCommandHandler with the given state type.
         * The handler receives state transition commands and routes them to the
         * appropriate StateManager.
         *
         * @tparam T The state enum type (e.g., GameState, MatchState).
         *
         * @param stateCommandHandler Reference to the handler to register.
         *
         * @return True if registration succeeded, false if already registered.
         *
         * @see StateManager
         * @see TypedStateCommandHandler
         */
        template<typename T>
        bool registerStateCommandHandler(
            helios::engine::state::TypedStateCommandHandler<T>& stateCommandHandler
        ) {
            const auto typeId = helios::engine::core::data::StateTypeId::id<T>();
            const auto idx = typeId.value();

            if (idx >= stateCommandHandlers_.size()) {
                stateCommandHandlers_.resize(idx + 1);
            }


            assert(!stateCommandHandlers_[idx] && "stateCommandHandler already registered");

            stateCommandHandlers_[idx] = &stateCommandHandler;

            return true;
        }

        /**
         * @brief Retrieves the registered state command handler for a type.
         *
         * @tparam T The state enum type to look up.
         *
         * @return Pointer to the handler, or nullptr if not registered.
         */
        template<typename T>
        [[nodiscard]] helios::engine::state::TypedStateCommandHandler<T>* stateCommandHandler() {
            const auto typeId = helios::engine::core::data::StateTypeId::id<T>();

            const auto idx = typeId.value();

            if (idx >= stateCommandHandlers_.size()) {
                return nullptr;
            }

            return static_cast< helios::engine::state::TypedStateCommandHandler<T>*>(stateCommandHandlers_[idx]);
        }

        /**
         * @brief Retrieves a registered Manager by type.
         *
         * @tparam T The Manager type to retrieve. Must derive from Manager.
         *
         * @return Pointer to the Manager if found, nullptr otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<helios::engine::runtime::world::Manager, T>
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
        void flushManagers(helios::engine::runtime::world::UpdateContext& updateContext) {
            for (auto& mgr :  managers_) {
                mgr->flush(*this, updateContext);
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
            return helios::engine::ecs::View<Components...>(&em_);
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

            for (auto& mgr : managers_) {
                mgr->reset();
            }

            session_.reset();

        }



    };

}

