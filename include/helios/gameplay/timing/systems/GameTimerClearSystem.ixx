/**
 * @file GameTimerClearSystem.ixx
 * @brief System that resets finished game timers each frame.
 */
module;


export module helios.gameplay.timing.systems.GameTimerClearSystem;

import helios.gameplay.timing.GameTimer;
import helios.gameplay.timing.TimerManager;

import helios.runtime.world.UpdateContext;

import helios.runtime.world.tags.SystemRole;

import helios.gameplay.timing.types;

using namespace helios::gameplay::timing;

using namespace helios::gameplay::timing::types;

export namespace helios::gameplay::timing::systems {

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
     * @see GameTimer
     */
    class GameTimerClearSystem {

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
        explicit GameTimerClearSystem(TimerManager& timerManager)
        : timerManager_(timerManager) {}

        /**
         * @brief Resets all finished timers to TimerState::Undefined.
         *
         * @param updateContext The current frame's update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto& gameTimer : timerManager_.gameTimers()) {
                if (gameTimer.state() == TimerState::Finished || gameTimer.state() == TimerState::Cancelled) {
                    gameTimer.setState(TimerState::Undefined);
                }
            }
        }

    };

}
