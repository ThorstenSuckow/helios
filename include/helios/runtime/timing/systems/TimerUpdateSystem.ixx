/**
 * @file TimerUpdateSystem.ixx
 * @brief System that advances all game timers each frame.
 */
module;


export module helios.runtime.timing.systems.TimerUpdateSystem;

import helios.runtime.timing.Timer;
import helios.runtime.timing.TimerManager;

import helios.state.Bindings;


import helios.runtime.world.UpdateContext;
import helios.runtime.messaging.command.NullCommandBuffer;
import helios.runtime.messaging.command.concepts.IsCommandBufferLike;

import helios.runtime.world.tags.SystemRole;

import helios.runtime.timing.types;
import helios.runtime.timing.commands;

using namespace helios::runtime::timing;

using namespace helios::runtime::timing::types;
using namespace helios::runtime::timing::commands;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command;
using namespace helios::runtime::messaging::command::concepts;

export namespace helios::runtime::timing::systems {

    /**
     * @brief Updates all game timers managed by the TimerManager.
     *
     * Each frame, this system iterates over all registered timers and
     * advances their elapsed time by the current delta time.
     *
     * @see TimerManager
     * @see Timer
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class TimerUpdateSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TimerManager& timerManager_;

    public:


        using EngineRoleTag = helios::runtime::tags::SystemRole;
        using CommandBuffer_type = TCommandBuffer;


        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The manager whose timers are updated.
         */
        explicit TimerUpdateSystem(TimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Advances all game timers by the current delta time.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            for (auto& timer : timerManager_.timers()) {
                if (timer.state() == TimerState::Running) {

                    timer.update(updateContext.deltaTime());

                    if (timer.duration() != 0.0f && timer.elapsed() >= timer.duration()) {
                        auto context = TimerControlContext{timer.timerId(), TimerState::Finished};
                        cmdBuffer.template add<TimerControlCommand>(context);
                    }
                }
            }
        }

    };

}
