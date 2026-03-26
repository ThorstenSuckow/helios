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

using namespace helios::engine::mechanics::timing::types;

export namespace helios::examples::scoring {

    inline void configureTimer(
        helios::engine::mechanics::timing::TimerManager& timerManager
    ) {
        using namespace helios::engine::mechanics::timing;

        auto& demoGameTimer = timerManager.addGameTimer(IdConfig::DemoTimerId);
        demoGameTimer.setDuration(60 * 3);
        demoGameTimer.setState(TimerState::Cancelled);

        auto& countdownTimer = timerManager.addGameTimer(IdConfig::CountdownTimerId);
        countdownTimer.setDuration(4);
        countdownTimer.setState(TimerState::Cancelled);

        auto& playerRespawnTimerId = timerManager.addGameTimer(IdConfig::PlayerRespawnTimerId);
        playerRespawnTimerId.setDuration(3);
        playerRespawnTimerId.setState(TimerState::Cancelled);
    }


}