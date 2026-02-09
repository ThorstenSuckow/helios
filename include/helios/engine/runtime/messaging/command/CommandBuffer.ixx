/**
 * @file CommandBuffer.ixx
 * @brief Queue for deferred execution of game commands.
 */
module;

#include <cassert>
#include <deque>
#include <format>
#include <memory>
#include <typeindex>
#include <unordered_map>

export module helios.engine.runtime.messaging.command.CommandBuffer;

import helios.engine.runtime.messaging.command.TargetedCommand;
import helios.engine.runtime.messaging.command.TargetedCommandDispatcher;
import helios.engine.runtime.messaging.command.TypedTargetedCommandDispatcher;

import helios.engine.runtime.messaging.command.WorldCommand;
import helios.engine.runtime.messaging.command.WorldCommandDispatcher;
import helios.engine.runtime.messaging.command.TypedWorldCommandDispatcher;

import helios.engine.runtime.world.GameWorld;
import helios.engine.ecs.GameObject;
import helios.engine.ecs.EntityHandle;
import helios.util.Guid;

import helios.util.log.Logger;
import helios.util.log.LogManager;



#define HELIOS_LOG_SCOPE "helios::engine::runtime::messaging::command::CommandBuffer"
export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Queue for deferred execution of game commands.
     *
     * @details The `CommandBuffer` provides a queue for game commands, decoupling their
     * creation from their execution. Commands are buffered along with their target
     * `GameObject` identifiers (`Guid`) and are executed in a single batch when `flush()`
     * is called against a `GameWorld`.
     *
     * Typical usage pattern:
     * ```cpp
     * helios::engine::runtime::messaging::command::CommandBuffer cmdBuffer;
     * helios::engine::runtime::world::GameWorld world;
     *
     * // Queue commands during input processing
     * cmdBuffer.add(player.guid(), std::make_unique<MoveCommand>(direction, speed));
     * cmdBuffer.add(enemy.guid(), std::make_unique<AttackCommand>(target));
     *
     * // Execute all commands at end of frame
     * cmdBuffer.flush(world);  // Executes and clears buffer
     * ```
     *
     * @note Commands targeting non-existent GameObjects are silently skipped during flush().
     *       This behavior will be logged once the logging system is available.
     */
    class CommandBuffer {

        /**
         * @brief Internal struct pairing a command with its target GameObject identifier.
         */
        struct TargetedCommandProxy {
            std::type_index typeIdx;
            helios::engine::ecs::EntityHandle entityHandle;
            std::unique_ptr<helios::engine::runtime::messaging::command::TargetedCommand> targetedCommand;
        };

        /**
         * @brief Internal struct pairing a world command with its type index.
         */
        struct WorldCommandProxy {
            std::type_index typeIdx;
            std::unique_ptr<helios::engine::runtime::messaging::command::WorldCommand> worldCommand;
        };

        /**
         * @brief Internal queue storing buffered commands.
         *
         * @details Uses std::deque for efficient push_back and iteration.
         */
        std::deque<TargetedCommandProxy> targetedCommandBuffer_;

        /**
         * @brief Internal queue storing buffered world commands.
         *
         * @details Uses std::deque for efficient push_back and iteration.
         */
        std::deque<WorldCommandProxy> worldCommandBuffer_;

        /**
         * @brief The logger used with this CommandBuffer instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

        /**
         * @brief Registry of dispatchers for TargetedCommand types.
         *
         * @details Maps command type indices to their corresponding dispatchers.
         * When a command is flushed and a dispatcher is registered for its type,
         * the command is routed through the dispatcher instead of executing directly.
         */
        std::unordered_map<std::type_index, std::unique_ptr<helios::engine::runtime::messaging::command::TargetedCommandDispatcher>> targetedCommandDispatchers_;

        /**
         * @brief Registry of dispatchers for WorldCommand types.
         *
         * @details Maps command type indices to their corresponding dispatchers.
         * When a command is flushed and a dispatcher is registered for its type,
         * the command is routed through the dispatcher instead of executing directly.
         */
        std::unordered_map<std::type_index, std::unique_ptr<helios::engine::runtime::messaging::command::WorldCommandDispatcher>> worldCommandDispatchers_;

    public:

        /**
         * @brief Registers a dispatcher for a specific TargetedCommand type.
         *
         * @details When flush() processes a command of type T, it will route the
         * command through this dispatcher instead of calling execute() directly.
         *
         * @tparam T The concrete TargetedCommand type to dispatch.
         *
         * @param d The dispatcher instance to register. Ownership is transferred.
         *
         * @pre No dispatcher for type T must already be registered.
         *
         * @return A reference to **this** CommandBuffer.
         */
        template<typename T>
        requires std::is_base_of_v<TargetedCommand, T>
        CommandBuffer& addDispatcher(std::unique_ptr<helios::engine::runtime::messaging::command::TypedTargetedCommandDispatcher<T>> d) {

            assert(!hasDispatcher<T>() && "Dispatcher already added");

            const std::type_index key{ typeid(T) };
            targetedCommandDispatchers_[key] = std::move(d);

            return *this;
        }

        /**
         * @brief Checks if a dispatcher is registered for a specific TargetedCommand type.
         *
         * @tparam T The concrete TargetedCommand type to check.
         *
         * @return true if a dispatcher is registered for type T, false otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<TargetedCommand, T>
        [[nodiscard]] bool hasDispatcher() const {
            auto it = targetedCommandDispatchers_.find(typeid(T));

            return it != targetedCommandDispatchers_.end();
        }

        /**
         * @brief Registers a dispatcher for a specific WorldCommand type.
         *
         * @details When flush() processes a command of type T, it will route the
         * command through this dispatcher instead of calling execute() directly.
         *
         * @tparam T The concrete WorldCommand type to dispatch.
         *
         * @param d The dispatcher instance to register. Ownership is transferred.
         *
         * @pre No dispatcher for type T must already be registered.
         *
         * @return A reference to **this** CommandBuffer.
         */
        template<typename T>
        requires std::is_base_of_v<WorldCommand, T>
        CommandBuffer& addDispatcher(std::unique_ptr<helios::engine::runtime::messaging::command::TypedWorldCommandDispatcher<T>> d) {

            assert(!hasDispatcher<T>() && "Dispatcher already added");

            const std::type_index key{ typeid(T) };
            worldCommandDispatchers_[key] = std::move(d);

            return *this;
        }

        /**
         * @brief Checks if a dispatcher is registered for a specific WorldCommand type.
         *
         * @tparam T The concrete WorldCommand type to check.
         *
         * @return true if a dispatcher is registered for type T, false otherwise.
         */
        template<typename T>
        requires std::is_base_of_v<WorldCommand, T>
        [[nodiscard]] bool hasDispatcher() const {
            auto it = worldCommandDispatchers_.find(typeid(T));

            return it != worldCommandDispatchers_.end();
        }

        /**
         * @brief Adds a command targeting a GameObject identified by Guid.
         *
         * @param guid The unique identifier of the target GameObject.
         * @param command The command to execute. Ownership is transferred to the buffer.
         *
         * @pre command must not be nullptr.
         *
         * @note If command is nullptr, the call is silently ignored (will be logged in future).
         * @note This overload is useful when the GameObject instance is not directly available,
         *       such as in networked scenarios or when processing serialized commands.
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<TargetedCommand, T>
        void add(const helios::engine::ecs::EntityHandle& entityHandle, Args&& ...args) {

            const std::type_index key{ typeid(T) };
            auto cmd = std::make_unique<T>(std::forward<Args>(args)...);

            targetedCommandBuffer_.push_back(TargetedCommandProxy{key, entityHandle, std::move(cmd)});
        }

        /**
         * @brief Adds a world command to the buffer.
         *
         * @tparam T The concrete WorldCommand type to create.
         * @tparam Args Constructor argument types for T.
         *
         * @param args Arguments forwarded to the T constructor.
         *
         * @details WorldCommands operate on the entire GameWorld rather than a
         * specific GameObject. They are executed before TargetedCommands during flush().
         */
        template<typename T, typename... Args>
        requires std::is_base_of_v<WorldCommand, T>
        void add(Args&& ...args) {

            const std::type_index key{ typeid(T) };
            auto cmd = std::make_unique<T>(std::forward<Args>(args)...);

            worldCommandBuffer_.push_back(WorldCommandProxy{key, std::move(cmd)});
        }

        /**
         * @brief Executes all buffered commands against the GameWorld and clears the buffer.
         *
         * @param gameWorld The world containing target GameObjects.
         *
         * @return Reference to this CommandBuffer for method chaining.
         *
         * @details Execution proceeds in two phases:
         * 1. **WorldCommands** are processed first. For each command, if a dispatcher
         *    is registered for its type, the command is routed via accept(). Otherwise,
         *    execute() is called directly.
         * 2. **TargetedCommands** are processed second. Each command's target GameObject
         *    is resolved by Guid. If found and a dispatcher exists, the command is routed
         *    via accept(). Otherwise, execute() is called directly.
         *
         * @note Commands targeting non-existent GameObjects (i.e., find() returns nullptr)
         *       are skipped and a warning is logged.
         * @note All commands are cleared after execution, regardless of success or failure.
         *
         * @warning Command execution should be noexcept. If execute() throws, the flush
         *          operation aborts and remaining commands are not executed.
         */
        CommandBuffer& flush(helios::engine::runtime::world::GameWorld& gameWorld) {

            // World commands are processed first
            for (auto& worldCommandProxy : worldCommandBuffer_) {

                auto it = worldCommandDispatchers_.find(worldCommandProxy.typeIdx);

                if (it != worldCommandDispatchers_.end()) {
                    worldCommandProxy.worldCommand->accept(gameWorld, *(it->second));
                } else {
                    worldCommandProxy.worldCommand->execute(gameWorld);
                }
            }

            // Targeted commands are processed second
            for (auto& targetedCommandProxy : targetedCommandBuffer_) {

                auto gameObject = gameWorld.find(targetedCommandProxy.entityHandle);

                if (!gameObject) {
                    logger_.warn("GameObject with entityHandle not found, skipping command");
                    continue;
                }

                auto it = targetedCommandDispatchers_.find(targetedCommandProxy.typeIdx);

                if (it != targetedCommandDispatchers_.end()) {
                    targetedCommandProxy.targetedCommand->accept(*gameObject, *(it->second));
                } else {
                    targetedCommandProxy.targetedCommand->execute(*gameObject);
                }
            }

            clear();
            return *this;
        }

        /**
         * @brief Clears all buffered commands without executing them.
         *
         * @return Reference to this CommandBuffer for method chaining.
         *
         * @note This destroys all buffered commands. Use this to discard commands
         *       without executing them (e.g., when reverting to a previous game state).
         */
        CommandBuffer& clear() {
            targetedCommandBuffer_.clear();
            worldCommandBuffer_.clear();
            return *this;
        }
    };


}