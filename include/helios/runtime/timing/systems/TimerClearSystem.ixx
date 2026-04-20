/**
 * @file TimerClearSystem.ixx
 * @brief System that resets finished game timers each frame.
 */
module;


export module helios.runtime.timing.systems.TimerClearSystem;

import helios.runtime.timing.Timer;
import helios.runtime.timing.TimerManager;

import helios.runtime.world.UpdateContext;

import helios.runtime.world.tags.SystemRole;

import helios.runtime.timing.types;

using namespace helios::runtime::timing;

using namespace helios::runtime::timing::types;

export namespace helios::runtime::timing::systems {

    /**
     * @brief Resets finished game timers to an undefined state.
     *
     * @details Each frame, this system iterates over all registered timers
     * and resets any timer whose state is `TimerState::Finished` back to
     * `TimerState::Undefined`. This ensures that one-shot timer completions
     * are consumed within a single frame and do not trigger downstream
     * logic more than once.
     *
     * @see TimerManager
     * @see Timer
     */
    class TimerClearSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TimerManager& timerManager_;

    public:


        using EngineRoleTag = helios::runtime::tags::SystemRole;

        /**
         * @brief Constructs the system with a reference to the TimerManager.
         *
         * @param timerManager The manager whose timers are cleared.
         */
        explicit TimerClearSystem(TimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Resets all finished timers to TimerState::Undefined.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto& timer : timerManager_.timers()) {
                if (timer.state() == TimerState::Finished || timer.state() == TimerState::Cancelled) {
                    timer.setState(TimerState::Undefined);
                }
            }
        }

    };

}
