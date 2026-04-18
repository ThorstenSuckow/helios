/**
 * @file TypedCommandBuffer.ixx
 * @brief Compile-time typed command buffer with handler routing.
 */
module;


#include <cassert>
#include <iostream>
#include <ostream>
#include <tuple>
#include <utility>
#include <vector>

export module helios.runtime.messaging.command.TypedCommandBuffer;

import helios.state.components;

import helios.runtime.world.GameWorld;
import helios.runtime.world.UpdateContext;

import helios.state.commands.DelayedStateCommand;

import helios.gameplay.timing.TimerManager;
import helios.gameplay.timing.types;

using namespace helios::runtime::world;
using namespace helios::state::commands;
using namespace helios::gameplay::timing;
using namespace helios::gameplay::timing::types;

import helios.engine.common.tags.CommandBufferRole;
export namespace helios::runtime::messaging::command {

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
     * @brief Concept constraining commands that carry a timer gate.
     *
     * @details A command satisfies DelayedCommandLike if it provides a
     * noexcept `gameTimerId()` accessor returning a GameTimerId. Such
     * commands are held in a scratch queue until their associated timer
     * reaches `TimerState::Finished`.
     *
     * @tparam Cmd The command type to check.
     */
    template<typename Cmd>
    concept DelayedCommandLike = requires(Cmd const& c) {
            {c.gameTimerId() } noexcept;
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
    class TypedCommandBuffer {

        /**
         * @brief Per-type command queues stored as a tuple of vectors.
         */
        std::tuple<std::vector<CommandTypes>...> commandQueues_;

        /**
         * @brief Scratch queues for timer-gated commands surviving a flush cycle.
         *
         * @details Commands satisfying DelayedCommandLike whose timer has
         * not yet finished are moved here during flush and swapped back
         * into the primary queues afterwards.
         */
        std::tuple<std::vector<CommandTypes>...> delayedQueues_;

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
         * @brief Returns the delayed scratch queue for a specific command type.
         *
         * @tparam CommandType The command type.
         *
         * @return Reference to the delayed scratch queue.
         */
        template<typename CommandType>
        std::vector<CommandType>& delayedQueue() noexcept {
            return std::get<std::vector<CommandType>>(delayedQueues_);
        }

        /**
         * @brief Determines whether a delayed command should be deferred.
         *
         * @details Returns true when the timer is still running - i.e. its
         * state is neither Finished nor Undefined.
         *
         * @param state The current timer state.
         *
         * @return True if the command must remain in the scratch queue.
         */
        [[nodiscard]] bool shouldDelayCommand(const TimerState state) const noexcept {
            return state == TimerState::Running;
        }

        /**
         * @brief Determines whether a delayed command should be discarded.
         *
         * @details Returns true when the timer is not in state Running or Finished..
         *
         * @param state The current timer state.
         *
         * @return True if the command should be discarded.
         */
        [[nodiscard]] bool shouldDiscardCommand(const TimerState state) const noexcept {
            return state != TimerState::Running  && state != TimerState::Finished;;
        }

        /**
         * @brief Determines whether a delayed command is ready for dispatch.
         *
         * @param state The current timer state.
         *
         * @return True if the associated timer has finished.
         */
        [[nodiscard]] bool isDelayedCommandReady(const TimerState state) const noexcept {
            return state == TimerState::Finished;
        }

        /**
         * @brief Flushes a single command queue.
         *
         * @details Processing follows two branches depending on whether a
         * handler is registered for `CommandType`:
         *
         * **Handler registered** (`CommandHandlerRegistry::has<CommandType>()`):
         * - Each command is forwarded via `commandHandlerRegistry.submit<CommandType>(cmd)`.
         *
         * **No handler registered** (fallback):
         * - The command must satisfy `ExecutableCommand`; otherwise an
         *   assertion fires at runtime.
         * - Each command is dispatched via `cmd.execute(updateContext)`.
         *
         * In both branches, if `CommandType` satisfies `DelayedCommandLike`,
         * an additional timer check is performed per command:
         *
         * 1. The associated `GameTimer` is looked up via the command's
         *    `gameTimerId()`.
         * 2. If the timer is still running (`shouldDelayCommand` returns
         *    true), the command is moved into the scratch queue and
         *    survives the current flush cycle.
         * 3. If the timer has finished (`isDelayedCommandReady` returns
         *    true), the command is dispatched normally.
         * 4. If the timer state is `Undefined` (e.g. timer was removed),
         *    the command is silently dropped.
         *
         * After all commands have been processed the primary queue is
         * cleared, the scratch queue contents are swapped back in as
         * the new primary queue, and the scratch queue is cleared.
         *
         * @tparam CommandType The command type to flush.
         *
         * @param gameWorld The game world providing the CommandHandlerRegistry
         *                  and TimerManager.
         * @param updateContext The current frame's update context.
         */
        template<typename CommandType>
        void flushCommandQueue(GameWorld& gameWorld, UpdateContext& updateContext) noexcept {

            auto& timerManager = gameWorld.manager<TimerManager>();

            auto& queue = commandQueue<CommandType>();
            auto& delayed = delayedQueue<CommandType>();
            delayed.clear();

            if (queue.empty()) {
                return;
            }

            auto& commandHandlerRegistry = gameWorld.commandHandlerRegistry();

            if (commandHandlerRegistry.has<CommandType>()) {

                for (auto& cmd : queue) {
                    if constexpr (DelayedCommandLike<CommandType>)  {
                        auto* gameTimer = timerManager.gameTimer(cmd.gameTimerId());
                        if (!gameTimer) {
                            assert(gameTimer && "Unexpected null game timer");
                            commandHandlerRegistry.submit<CommandType>(cmd);
                            continue;
                        }

                        if (shouldDelayCommand(gameTimer->state())) {
                            delayed.push_back(std::move(cmd));
                        } else if (isDelayedCommandReady(gameTimer->state())) {
                            commandHandlerRegistry.submit<CommandType>(cmd);
                        } else if (shouldDiscardCommand(gameTimer->state())) {
                            // cancelled? Discard! intentionally noop
                        }
                    } else {
                        commandHandlerRegistry.submit<CommandType>(cmd);
                    }
                }


            } else {
               if constexpr (ExecutableCommand<CommandType>) {

                   for (auto& cmd : queue) {
                      if constexpr (DelayedCommandLike<CommandType>)  {
                           auto* gameTimer = timerManager.gameTimer(cmd.gameTimerId());
                           if (!gameTimer) {
                               assert(gameTimer && "Unexpected null game timer");
                               cmd.execute(updateContext);
                               continue;
                           }

                           if (shouldDelayCommand(gameTimer->state())) {
                               delayed.push_back(std::move(cmd));
                           } else if (isDelayedCommandReady(gameTimer->state())) {
                               cmd.execute(updateContext);
                           } else if (shouldDiscardCommand(gameTimer->state())) {
                               // cancelled? Discard! intentionally noop
                           }
                       } else {
                           cmd.execute(updateContext);
                       }

                   }

               } else {
                   std::cerr << "Command type is not executable" << __FUNCSIG__ << std::endl;
                   assert(false &&  "Command type is not executable");
               }

            }

            queue.clear();
            queue.swap(delayed);
            delayed.clear();
        }

    public:

        using EngineRoleTag = helios::engine::common::tags::CommandBufferRole;

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
        void clear() noexcept {
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
        void flush(GameWorld& gameWorld,  UpdateContext& updateContext) noexcept {
            (flushCommandQueue<CommandTypes>(gameWorld, updateContext), ...);
        }


    };


}