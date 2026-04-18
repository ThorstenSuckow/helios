/**
 * @file GameTimerUpdateSystem.ixx
 * @brief System that advances all game timers each frame.
 */
module;


export module helios.gameplay.timing.systems.GameTimerUpdateSystem;

import helios.gameplay.timing.GameTimer;
import helios.gameplay.timing.TimerManager;

import helios.state.Bindings;


import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.engine.common.concepts.IsCommandBufferLike;

import helios.engine.common.tags.SystemRole;

import helios.gameplay.timing.types;
import helios.gameplay.timing.commands;

using namespace helios::gameplay::timing;

using namespace helios::gameplay::timing::types;
using namespace helios::gameplay::timing::commands;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::engine::common::concepts;

export namespace helios::gameplay::timing::systems {

    /**
     * @brief Updates all game timers managed by the TimerManager.
     *
     * Each frame, this system iterates over all registered timers and
     * advances their elapsed time by the current delta time.
     *
     * @see TimerManager
     * @see GameTimer
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class GameTimerUpdateSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TimerManager& timerManager_;

    public:


        using EngineRoleTag = helios::engine::common::tags::SystemRole;


        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The manager whose timers are updated.
         */
        explicit GameTimerUpdateSystem(TimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Advances all game timers by the current delta time.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto& gameTimer : timerManager_.gameTimers()) {
                if (gameTimer.state() == TimerState::Running) {

                    gameTimer.update(updateContext.deltaTime());

                    if (gameTimer.duration() != 0.0f && gameTimer.elapsed() >= gameTimer.duration()) {
                        auto context = TimerControlContext{gameTimer.gameTimerId(), TimerState::Finished};
                        updateContext.queueCommand<TCommandBuffer, TimerControlCommand>(context);
                    }
                }
            }
        }

    };

}
