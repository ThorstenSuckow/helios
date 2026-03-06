/**
 * @file TypedCommandBuffer.ixx
 * @brief Compile-time typed command buffer with handler routing.
 */
module;

#include <cassert>
#include <tuple>
#include <vector>
#include <utility>

export module helios.engine.runtime.messaging.command.TypedCommandBuffer;

import helios.engine.state.components;

import helios.engine.runtime.world.GameWorld;
import helios.engine.runtime.world.UpdateContext;

import helios.engine.runtime.messaging.command.CommandBuffer;

using namespace helios::engine::runtime::world;

export namespace helios::engine::runtime::messaging::command {

    /**
     * @brief Concept constraining commands that can self-execute.
     *
     * @details A command satisfies ExecutableCommand if it provides a
     * noexcept `execute(UpdateContext&)` method. Commands that do not
     * satisfy this concept must have a registered handler.
     *
     * @tparam Cmd The command type to check.
     */
    template<typename Cmd>
    concept ExecutableCommand = requires(Cmd const& c, UpdateContext& updateContext) {
        {c.execute(updateContext) } noexcept;
    };


    /**
     * @brief Compile-time typed command buffer with per-type queues and handler routing.
     *
     * @details TypedCommandBuffer stores commands in separate `std::vector` queues,
     * one per command type, packed into a `std::tuple`. This provides:
     *
     * - **Zero-overhead dispatch:** Command types are known at compile time,
     *   eliminating virtual dispatch for queue access.
     * - **Deterministic ordering:** Commands are flushed in the order of the
     *   template parameter list, ensuring reproducible execution.
     * - **Handler-or-execute routing:** During flush, each command is either
     *   routed to a registered handler or executed directly via
     *   its `execute()` method (if it satisfies ExecutableCommand).
     *
     * ## Flush Routing
     *
     * For each command type in the parameter pack:
     * 1. If a handler for `Cmd` is registered → `handler.submit(cmd)`
     * 2. Else if `Cmd` satisfies `ExecutableCommand` → `cmd.execute(ctx)`
     * 3. Else → assertion failure (misconfiguration)
     *
     * @tparam CommandTypes The command types this buffer manages.
     *
     * @see CommandBuffer
     * @see CommandHandlerRegistry
     * @see EngineCommandBuffer
     * @see ExecutableCommand
     */
    template <typename... CommandTypes>
    class TypedCommandBuffer : public CommandBuffer {

        /**
         * @brief Per-type command queues stored as a tuple of vectors.
         */
        std::tuple<std::vector<CommandTypes>...> commandQueues_;

        /**
         * @brief Returns the queue for a specific command type.
         *
         * @tparam CommandType The command type.
         *
         * @return Reference to the command queue.
         */
        template<typename CommandType>
        std::vector<CommandType>& commandQueue() noexcept {
            return std::get<std::vector<CommandType>>(commandQueues_);
        }

        /**
         * @brief Flushes a single command queue.
         *
         * @details Routes each command to its registered handler, or
         * executes it directly if no handler is registered and the
         * command satisfies ExecutableCommand.
         *
         * @tparam CommandType The command type to flush.
         *
         * @param gameWorld The game world for which the queue should be flushed.
         * @param updateContext The current frame's update context.
         */
        template<typename CommandType>
        void flushCommandQueue(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {

            auto& queue = commandQueue<CommandType>();
            if (queue.empty()) {
                return;
            }

            auto& commandHandlerRegistry = gameWorld.commandHandlerRegistry();

            if (gameWorld.commandHandlerRegistry().has<CommandType>()) {
                for (auto& cmd : queue) {
                    commandHandlerRegistry.submit<CommandType>(cmd);
                }
            } else {
               if constexpr (ExecutableCommand<CommandType>) {
                   for (auto& cmd : queue) {
                       cmd.execute(updateContext);
                   }
               } else {
                   assert(false &&  "Command type is not executable");
               }

            }

            queue.clear();
        }

    public:


        /**
         * @brief Enqueues a command of the specified type.
         *
         * @tparam T The command type. Must be one of the CommandTypes.
         * @tparam Args Constructor argument types.
         *
         * @param args Arguments forwarded to the command constructor.
         */
        template<typename T, typename... Args>
        void add(Args&&... args) {

            auto& queue = std::get<std::vector<T>>(commandQueues_);
            queue.emplace_back(std::forward<Args>(args)...);

        }

        /**
         * @brief Discards all queued commands without executing them.
         */
        void clear() noexcept override {
            std::apply([](auto&... queue) { (queue.clear(), ...); }, commandQueues_);
        }

        /**
         * @brief Flushes all command queues in template parameter order.
         *
         * @details Iterates through each command type using a fold expression,
         * flushing queues in the order specified by the template parameters.
         *
         * @param gameWorld The game world for which the queue should be flushed.
         * @param updateContext The current frame's update context.
         */
        void flush(GameWorld& gameWorld,  UpdateContext& updateContext) noexcept override {
            (flushCommandQueue<CommandTypes>(gameWorld, updateContext), ...);
        }


    };


}