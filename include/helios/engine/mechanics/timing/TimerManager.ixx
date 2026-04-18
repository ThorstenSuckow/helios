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

export module helios.engine.mechanics.timing.TimerManager;

import helios.engine.mechanics.timing.types;
import helios.engine.mechanics.timing.commands;

import helios.engine.mechanics.timing.GameTimer;

import helios.engine.mechanics.timing.types.GameTimerId;

import helios.runtime.world.UpdateContext;

import helios.runtime.world.GameWorld;

import helios.core.types;
import helios.util.Guid;
import helios.engine.common;

using namespace helios::engine::mechanics::timing::commands;
using namespace helios::engine::mechanics::timing::types;
using namespace helios::engine::mechanics::timing::types;
using namespace helios::runtime::world;
export namespace helios::engine::mechanics::timing {

    /**
     * @brief Manager that owns game timers and processes timer control commands.
     *
     *
     * Pending control commands are collected via submit() and applied during
     * flush() at the beginning of each frame.
     *
     * @see GameTimer
     * @see TimerCommandHandler
     * @see Manager
     */
    class TimerManager {

        /**
         * @brief Collection of game timers managed by this manager.
         */
        std::vector<GameTimer> gameTimers_;

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
        [[nodiscard]] bool has(const GameTimerId timerId) noexcept {
            return getGameTimer(timerId) != nullptr;
        }

        /**
         * @brief Looks up a timer by its id.
         *
         * @param timerId The id to search for.
         *
         * @return Pointer to the timer, or nullptr if not found.
         */
        GameTimer* getGameTimer(const GameTimerId timerId) {
            const auto timer = std::ranges::find_if(
                gameTimers_,
                [&](const auto& gameTimer) {
                    return gameTimer.gameTimerId() == timerId;
                });

            if (timer == gameTimers_.end()) {
                return nullptr;
            }
            return &*timer;
        }

        /**
         * @brief Creates and appends a new timer.
         *
         * @param gameTimerId The id for the new timer.
         *
         * @return Reference to the newly created timer.
         */
        GameTimer& add(const GameTimerId gameTimerId) noexcept {
            gameTimers_.emplace_back(GameTimer(gameTimerId));

            return gameTimers_.back();
        }

    public:
        using EngineRoleTag = helios::engine::common::tags::ManagerRole;

        /**
         * @brief Registers a new game timer.
         *
         * Asserts that no timer with the given id already exists.
         *
         * @param gameTimerId The unique id for the new timer.
         *
         * @return Reference to the newly created GameTimer.
         */
        GameTimer& addGameTimer(GameTimerId gameTimerId) noexcept {
            assert(!has(gameTimerId) && "GameTimer with GameTimerId already registered");

            return add(gameTimerId);
        }

        /**
         * @brief Returns a pointer to the timer with the given id.
         *
         * @param gameTimerId The id to look up.
         *
         * @return Pointer to the GameTimer, or nullptr if not found.
         */
        [[nodiscard]] GameTimer* gameTimer(const GameTimerId gameTimerId) noexcept {
            return getGameTimer(gameTimerId);
        }

        /**
         * @brief Returns a span over all registered timers.
         *
         * @return A span of GameTimer instances.
         */
        [[nodiscard]] std::span<GameTimer> gameTimers() noexcept {
            return gameTimers_;
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
                auto* timer = gameTimer(controlContext.gameTimerId);
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
        void init(helios::runtime::world::GameWorld& gameWorld) {
            gameWorld.template registerCommandHandler<TimerControlCommand>(*this);
        }

        /**
         * @brief Resets all managed timers.
         */
        void reset() {
            for (auto& gameTimer : gameTimers_) {
                gameTimer.reset();
            }
        }
    };

}
