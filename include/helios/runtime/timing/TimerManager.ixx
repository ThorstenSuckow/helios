/**
 * @file TimerManager.ixx
 * @brief Manager for game timers and timer command handling.
 */
module;

#include <cassert>
#include <memory>
#include <span>
#include <vector>
#include <stdexcept>
#include <ranges>

export module helios.runtime.timing.TimerManager;

import helios.runtime.timing.types;
import helios.runtime.timing.commands;

import helios.runtime.timing.Timer;

import helios.runtime.timing.types.TimerId;

import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.CommandHandlerRegistry;

import helios.core.types;
import helios.util.Guid;
import helios.runtime.world.tags.ManagerRole;

using namespace helios::runtime::timing::commands;
using namespace helios::runtime::timing::types;
using namespace helios::runtime::timing::types;
using namespace helios::runtime::world;
export namespace helios::runtime::timing {

    /**
     * @brief Manager that owns game timers and processes timer control commands.
     *
     *
     * Pending control commands are collected via submit() and applied during
     * flush() at the beginning of each frame.
     *
     * @see Timer
     * @see TimerCommandHandler
     * @see Manager
     */
    class TimerManager {

        /**
         * @brief Collection of game timers managed by this manager.
         */
        std::vector<Timer> timers_;

        /**
         * @brief Pending timer control contexts to be applied during flush.
         */
        std::vector<TimerControlContext> pendingControlContexts_;

        /**
         * @brief Checks whether a timer with the given id exists.
         *
         * @param timerId The id to look up.
         *
         * @return True if a timer with the given id is registered.
         */
        [[nodiscard]] bool has(const TimerId timerId) noexcept {
            return getTimer(timerId) != nullptr;
        }


        /**
         * @brief Creates and appends a new timer.
         *
         * @param timerId The id for the new timer.
         *
         * @return Reference to the newly created timer.
         */
        Timer& add(const TimerId timerId) noexcept {
            timers_.emplace_back(Timer(timerId));

            return timers_.back();
        }

    public:
        using EngineRoleTag = helios::runtime::tags::ManagerRole;

        /**
         * @brief Registers a new game timer.
         *
         * Asserts that no timer with the given id already exists.
         *
         * @param timerId The unique id for the new timer.
         *
         * @return Reference to the newly created Timer.
         */
        Timer& addTimer(TimerId timerId) noexcept {
            assert(!has(timerId) && "Timer with TimerId already registered");

            return add(timerId);
        }

        /**
         * @brief Looks up a timer by its id.
         *
         * @param timerId The id to search for.
         *
         * @return Pointer to the timer, or nullptr if not found.
         */
        Timer* getTimer(const TimerId timerId) {
            const auto timerIt = std::ranges::find_if(
                timers_,
                [&](const auto& timer) {
                    return timer.timerId() == timerId;
                });

            if (timerIt == timers_.end()) {
                return nullptr;
            }
            return &*timerIt;
        }

        /**
         * @brief Returns a span over all registered timers.
         *
         * @return A span of Timer instances.
         */
        [[nodiscard]] std::span<Timer> timers() noexcept {
            return timers_;
        }

        /**
         * @brief Applies all pending timer control contexts.
         *
         * For each pending context, the corresponding timer's state is updated.
         * The pending list is cleared after processing.
         *
         * @param gameWorld Reference to the game world.
         * @param updateContext Reference to the current update context.
         */
        void flush(
            helios::runtime::world::UpdateContext& updateContext
        ) noexcept {

            for (const auto& controlContext : pendingControlContexts_) {
                auto* timer = getTimer(controlContext.timerId);
                if (timer) {
                    if (controlContext.resetElapsed) {
                        timer->reset(controlContext.timerState);
                    } else {
                        timer->setState(controlContext.timerState);
                    } 
                }
            }
            pendingControlContexts_.clear();
        }

        /**
         * @brief Enqueues a timer control command for deferred processing.
         *
         * @param timerControlCommand The command to enqueue.
         *
         * @return True if the command was accepted.
         */
        bool submit(const TimerControlCommand timerControlCommand) noexcept {
            pendingControlContexts_.push_back(timerControlCommand.timerControlContext());
            return true;
        };

        /**
         * @brief Registers this manager as the timer command handler in the game world.
         *
         * @param gameWorld The game world to register with.
         */
        void init(helios::runtime::messaging::command::CommandHandlerRegistry& commandHandlerRegistry) {
            commandHandlerRegistry.registerHandler<TimerControlCommand>(*this);
        }

        /**
         * @brief Resets all managed timers.
         */
        void reset() {
            for (auto& timer : timers_) {
                timer.reset();
            }
        }
    };

}
