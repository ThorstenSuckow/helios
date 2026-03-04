/**
 * @file GameTimerUpdateSystem.ixx
 * @brief System that advances all game timers each frame.
 */
module;

#include <span>


export module helios.engine.mechanics.timing.systems.GameTimerUpdateSystem;

import helios.engine.mechanics.timing.GameTimer;
import helios.engine.mechanics.timing.TimerManager;

import helios.engine.runtime.world.UpdateContext;


import helios.engine.runtime.world.GameWorld;

import helios.engine.mechanics.lifecycle.components.Active;
import helios.engine.mechanics.timing.components;

using namespace helios::engine::mechanics::timing;

export namespace helios::engine::mechanics::timing::systems {

    /**
     * @brief Updates all game timers managed by the TimerManager.
     *
     * Each frame, this system iterates over all registered timers and
     * advances their elapsed time by the current delta time.
     *
     * @see TimerManager
     * @see GameTimer
     */
    class GameTimerUpdateSystem {

        /**
         * @brief Reference to the TimerManager owning the timers.
         */
        TimerManager& timerManager_;

    public:

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
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            for (auto& gameTimer : timerManager_.gameTimers()) {
                gameTimer.update(updateContext.deltaTime());
            }

        }

    };

}
