/**
 * @file GameTimerConfiguration.ixx
 * @brief Timer configuration for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.GameTimerConfiguration;

import helios;
import helios.ext;
import helios.examples.scoring.IdConfig;

import helios.engine.mechanics.timing;

export namespace helios::examples::scoring {

    inline void configureTimer(
        helios::engine::mechanics::timing::TimerManager& timerManager
    ) {
        using namespace helios::engine::mechanics::timing;

        auto& demoGameTimer = timerManager.addGameTimer(IdConfig::DemoTimerId);
        demoGameTimer.setDuration(60 * 3);
        demoGameTimer.setState(TimerState::Stopped);
    }


}