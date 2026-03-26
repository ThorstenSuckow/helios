module;

#include <memory>

export module helios.examples.scoring.CountdownTimerListener;

import helios;

import helios.examples.scoring.IdConfig;

using namespace helios::engine::state;
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;
using namespace helios::engine::state::listeners;
using namespace helios::engine::runtime::world;
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::types;
using namespace helios::engine::mechanics::timing::commands;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;

export namespace helios::examples::scoring {

    inline void installCountdownTimeListeners(
        StateManager<GameState>& gameStateManager,
        StateManager<MatchState>& matchStateManager,
        TimerManager& timerManager
    ) {
        matchStateManager.addStateListener(
            std::make_unique<LambdaStateListener<MatchState>>(
            [&timerManager](UpdateContext& updateContext,
                const StateTransitionContext<MatchState> transitionContext) -> void {

                const auto to = transitionContext.to();
                const auto transitionId = transitionContext.transitionId();

                bool start = (transitionId == MatchStateTransitionId::CountdownRequest && to == MatchState::Countdown);

                if (start) {
                    auto* timer = timerManager.gameTimer(IdConfig::CountdownTimerId);
                    timer->reset();
                    timer->setState(TimerState::Running);
                    updateContext.queueCommand<DelayedStateCommand<MatchState>>(
                         StateTransitionRequest<MatchState>(
                             MatchState::Countdown,
                             MatchStateTransitionId::PlayerSpawnRequest
                         ), IdConfig::CountdownTimerId
                     );
                }
            })
        );
    }

}
