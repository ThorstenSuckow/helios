/**
 * @file CommandBuffer.ixx
 * @brief Queue for deferred execution of game commands.
 */
module;

#include <format>
#include <deque>
#include <memory>

export module helios.game.CommandBuffer;

import helios.game.Command;
import helios.game.GameWorld;
import helios.game.GameObject;
import helios.util.Guid;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::game::CommandBuffer"
export namespace helios::game {

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
     * helios::game::CommandBuffer cmdBuffer;
     * helios::game::GameWorld world;
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
        struct TargetedCommand {
            helios::util::Guid guid;
            std::unique_ptr<helios::game::Command> command;
        };

        /**
         * @brief Internal queue storing buffered commands.
         *
         * @details Uses std::deque for efficient push_back and iteration.
         */
        std::deque<TargetedCommand> commandBuffer_;

        /**
         * @brief The logger used with this CommandBuffer instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE);

    public:

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
        void add(const helios::util::Guid& guid, std::unique_ptr<helios::game::Command> command) {
            if (!command) {
                logger_.warn("Attempted to add a null command to the command buffer");
                return;
            }
            commandBuffer_.emplace_back(TargetedCommand{guid, std::move(command)});
        }

        /**
         * @brief Executes all buffered commands against the GameWorld and clears the buffer.
         *
         * @param gameWorld The world containing target GameObjects.
         *
         * @return Reference to this CommandBuffer for method chaining.
         *
         * @note Commands targeting non-existent GameObjects (i.e., find() returns nullptr)
         *       are skipped. This will be logged once the logging system is available.
         * @note All commands are cleared after execution, regardless of success or failure.
         *
         * @warning If a Command::execute() throws an exception, the flush operation will abort
         *          and remaining commands will not be executed. Consider making execute() noexcept
         *          or handling exceptions within command implementations.
         */
        CommandBuffer& flush(helios::game::GameWorld& gameWorld) {
            for (auto& targetedCommand : commandBuffer_) {
                auto* gameObject = gameWorld.find(targetedCommand.guid);
                if (!gameObject) {
                    logger_.warn(std::format("GameObject with Guid {} not found, skipping command",
                                             targetedCommand.guid.str()));
                    continue;
                }
                targetedCommand.command->execute(*gameObject);
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
            commandBuffer_.clear();
            return *this;
        }


    };


}