/**
 * @file GameWorld.ixx
 * @brief Central game state container for entities, resources, and the active level.
 */
module;

#include <cassert>
#include <format>
#include <helios/helios_config.h>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <cstddef>

export module helios.runtime.world.GameWorld;

import helios.runtime.world.Session;

import helios.ecs.Entity;
import helios.runtime.world.RuntimeEnvironment;
import helios.platform.environment.types;

import helios.runtime.messaging.command.CommandHandlerRegistry;

import helios.runtime.world.ResourceRegistry;

import helios.runtime.world.UpdateContext;
import helios.runtime.world.GameObject;
import helios.runtime.world.types.GameObjectId;
import helios.runtime.world.types.GameObjectHandle;
import helios.runtime.world.Manager;

import helios.util.Guid;
import helios.util.log.Logger;
import helios.util.log.LogManager;
import helios.runtime.world.Level;

import helios.ecs.types.EntityHandle;
import helios.ecs.EntityManager;
import helios.ecs.EntityRegistry;
import helios.ecs.View;

import helios.runtime.concepts;

import helios.runtime.world.EngineWorld;


using namespace helios::runtime::messaging::command::concepts;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::world::concepts;
using namespace helios::platform::environment::types;
using namespace helios::runtime::world::types;
#define HELIOS_LOG_SCOPE "GameWorld"
export namespace helios::runtime::world {

    /**
     * @brief Runtime root object coordinating world domains, resources, and frame services.
     *
     * @details `GameWorld` owns the `EngineWorld` (entity domains), `Session`,
     * `RuntimeEnvironment`, and runtime registries for managers/command buffers.
     * It also exposes typed convenience APIs (`add`, `find`, `view`, `destroy`)
     * that delegate to `EngineWorld`.
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
         * @brief Type-indexed registry for Managers and CommandBuffers.
         *
         * @details Provides O(1) type-based access via ManagerRegistry and
         * CommandBufferRegistry. Owns all registered Manager and CommandBuffer
         * instances via ConceptModelRegistry.
         */
        ResourceRegistry resourceRegistry_;

        /**
         * @brief Registry mapping command types to their handler function pointers.
         *
         * @details Used by TypedCommandBuffer during flush to route commands
         * to the correct handler. Handlers are registered via
         * `registerCommandHandler<CommandTypes...>(owner)`.
         */
        CommandHandlerRegistry commandHandlerRegistry_;

        /**
         * @brief Aggregate multi-domain entity world.
         */
        EngineWorld engineWorld_{};

        /**
         * @brief Session object storing runtime/game state.
         */
        Session session_;

        /**
         * @brief Runtime environment facade for platform readiness state.
         */
        RuntimeEnvironment runtimeEnvironment_;


    public:

        /**
         * @brief Constructs `GameWorld` and creates internal session/environment entities.
         *
         * @details The constructor creates one game-object entity for `Session`
         * and one platform entity for `RuntimeEnvironment`.
         */
        explicit GameWorld(const size_t capacity = ENTITY_MANAGER_DEFAULT_CAPACITY)
        : session_(Session(engineWorld_.add<GameObjectHandle>())),
         runtimeEnvironment_(RuntimeEnvironment(engineWorld_.add<PlatformHandle>()))
        {};

        /**
         * @brief Non-copyable, non-movable.
         */
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
         * @brief Returns a reference to the current runtime platform.
         *
         * @return Reference to the Platform.
         */
        [[nodiscard]] RuntimeEnvironment& runtimeEnvironment() {
            return runtimeEnvironment_;
        }



        /**
         * @brief Initializes all registered managers.
         *
         * @details Should be called after all managers have been added and before
         * the game loop starts. Each manager's init() method is invoked with a
         * reference to this GameWorld.
         */
        GameWorld& init() {
            for (auto& mgr :  resourceRegistry_.managers()) {
                mgr->init(*this);
            }

            return *this;
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
         * @brief Checks whether a Manager of type T is registered.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         *
         * @return True if the Manager is registered.
         */
        template<typename T>
        requires IsManagerLike<T>
        [[nodiscard]] bool hasManager() const {
            return resourceRegistry_.has<T>();
        }

        /**
         * @brief Checks whether a CommandBuffer of type T is registered.
         *
         * @tparam T The CommandBuffer type. Must satisfy IsCommandBufferLike.
         *
         * @return True if the CommandBuffer is registered.
         */
        template<typename T>
        requires IsCommandBufferLike<T>
        [[nodiscard]] bool hasCommandBuffer() const {
            return resourceRegistry_.has<T>();
        }

        /**
         * @brief Registers and constructs a Manager of type T.
         *
         * @details Delegates to ResourceRegistry::emplace. The Manager is
         * constructed in-place with forwarded arguments and owned by the
         * ManagerRegistry.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the newly registered Manager.
         */
        template<typename T, typename... Args>
        requires IsManagerLike<T>
        T& registerManager(Args&&... args) {
            return resourceRegistry_.emplace<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Registers and constructs a CommandBuffer of type T.
         *
         * @details Delegates to ResourceRegistry::emplace. The buffer is
         * constructed in-place with forwarded arguments and owned by the
         * CommandBufferRegistry.
         *
         * @tparam T The CommandBuffer type. Must satisfy IsCommandBufferLike.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @return Reference to the newly registered CommandBuffer.
         */
        template<typename T, typename... Args>
        requires IsCommandBufferLike<T>
        T& registerCommandBuffer(Args&&... args) {
            return resourceRegistry_.emplace<T>(std::forward<Args>(args)...);
        }

        /**
         * @brief Retrieves a registered Manager by type.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         *
         * @return Reference to the Manager.
         *
         * @pre A Manager of type T must be registered.
         */
        template<typename T>
        requires IsManagerLike<T>
        T& manager() const noexcept {
            assert(resourceRegistry_.has<T>() && "Manager not registered");
            return resourceRegistry_.get<T>();
        }

        /**
         * @brief Retrieves a registered Manager by type, or nullptr if not found.
         *
         * @tparam T The Manager type. Must satisfy IsManagerLike.
         *
         * @return Pointer to the Manager, or nullptr if not registered.
         */
        template<typename T>
        requires IsManagerLike<T>
        T* tryManager() const noexcept {
            return resourceRegistry_.tryGet<T>();
        }

        /**
         * @brief Retrieves a registered CommandBuffer by type, or nullptr if not found.
         *
         * @tparam T The CommandBuffer type. Must satisfy IsCommandBufferLike.
         *
         * @return Pointer to the CommandBuffer, or nullptr if not registered.
         */
        template<typename T>
        requires IsCommandBufferLike<T>
        T* tryCommandBuffer() const noexcept {
            return resourceRegistry_.tryGet<T>();
        }

        /**
         * @brief  Retrieves a registered CommandBuffer by type.
         *
         * @tparam T The CommandBuffer type. Must satisfy IsCommandBufferLike.
         *
         * @pre A CommandBuffer of type `T` must already be registered.
         *      Use tryCommandBuffer<T>() when the buffer is optional.
         *
         * @return Reference to the CommandBuffer.
         */
        template<typename T>
        requires IsCommandBufferLike<T>
        T& commandBuffer() const noexcept {
            return resourceRegistry_.get<T>();
        }


        /**
         * @brief Registers a command handler for one or more command types.
         *
         * @details Stores a type-erased function pointer for each CommandType
         * that routes to `owner.submit(cmd)`. During flush, the
         * TypedCommandBuffer uses the CommandHandlerRegistry to dispatch
         * queued commands to the registered handler.
         *
         * @tparam CommandType The command types to register handlers for.
         * @tparam OwningT The handler type. Must satisfy IsCommandHandlerLike.
         *
         * @param owner Reference to the handler instance. Must outlive the GameWorld.
         *
         * @see CommandHandlerRegistry
         */
        template<typename... CommandType, typename OwningT>
        requires IsCommandHandlerLike<OwningT, CommandType...>
        void registerCommandHandler(OwningT& owner) {
            (commandHandlerRegistry_.template registerHandler<CommandType>(owner), ...);
        }

        /**
         * @brief Returns a reference to the CommandHandlerRegistry.
         *
         * @return Reference to the CommandHandlerRegistry.
         */
        [[nodiscard]] CommandHandlerRegistry& commandHandlerRegistry() noexcept {
            return commandHandlerRegistry_;
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
         * @details Use for direct resource access. Prefer the convenience methods
         * `registerManager()`, `registerCommandBuffer()`, `manager()`,
         * `tryManager()`, and `tryCommandBuffer()` for type-constrained access.
         *
         * @return Reference to the ResourceRegistry.
         */
        ResourceRegistry& resourceRegistry() noexcept {
            return resourceRegistry_;
        }

        /**
         * @copydoc resourceRegistry()
         */
        const ResourceRegistry& resourceRegistry() const noexcept {
            return resourceRegistry_;
        }

        /**
         * @brief Returns the platform-domain world.
         *
         * @return Reference to `PlatformWorld`.
         */
        [[nodiscard]] PlatformWorld& platformWorld() noexcept{
            return engineWorld_.platformWorld();
        }

        /**
         * @brief Returns the render-resource domain world.
         *
         * @return Reference to `RenderResourceWorld`.
         */
        [[nodiscard]] RenderResourceWorld& renderResourceWorld() noexcept{
            return engineWorld_.renderResourceWorld();
        }

        /**
         * @brief Returns the render-target domain world.
         *
         * @return Reference to `RenderTargetWorld`.
         */
        [[nodiscard]] RenderTargetWorld& renderTargetWorld() noexcept{
            return engineWorld_.renderTargetWorld();
        }

        /**
         * @brief Returns the game-object domain world.
         *
         * @return Reference to `GameObjectWorld`.
         */
        [[nodiscard]] GameObjectWorld& gameObjectWorld() noexcept{
            return engineWorld_.gameObjectWorld();
        }


        /**
         * @brief Returns the aggregate typed world used for handle-routed operations.
         *
         * @return Reference to `EngineWorld`.
         */
        [[nodiscard]] EngineWorld& engineWorld() noexcept{
            return engineWorld_;
        }

        /**
         * @brief Builds a typed ECS view for a handle domain and component set.
         *
         * @tparam THandle Handle domain type.
         * @tparam Components Component types to include.
         *
         * @return Domain-specific view.
         */
        template <typename THandle, typename... Components>
        [[nodiscard]] auto view() {
            return engineWorld_.view<THandle, Components...>();
        }

        /**
         * @brief Finds an entity facade by handle.
         *
         * @tparam THandle Handle type.
         *
         * @param handle Entity handle to resolve.
         *
         * @return Domain-specific entity facade (or empty facade if not found).
         */
        template<typename THandle>
        [[nodiscard]] auto find(const THandle handle) noexcept {
            return engineWorld_.template find<THandle>(handle);
        }

        /**
         * @brief Adds a new entity in the domain inferred from `THandle`.
         *
         * @tparam THandle Handle type.
         *
         * @param strongId Optional strong id value used by the handle domain.
         *
         * @return Domain-specific entity facade for the created entity.
         */
        template<typename THandle>
        [[nodiscard]] auto add(const typename THandle::StrongId_type strongId = typename THandle::StrongId_type{}) noexcept {
            return engineWorld_.template add<THandle>(strongId);
        }

        /**
         * @brief Destroys an entity in the domain inferred from `THandle`.
         *
         * @tparam THandle Handle type.
         *
         * @param handle Entity handle to destroy.
         *
         * @return Domain-specific destroy result.
         */
        template<typename THandle>
        [[nodiscard]] auto destroy(const THandle handle) noexcept {
            return engineWorld_.template destroy<THandle>(handle);
        }

    };

}

