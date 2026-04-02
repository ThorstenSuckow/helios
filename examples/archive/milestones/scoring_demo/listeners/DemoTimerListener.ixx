module;

#include <memory>

export module helios.examples.scoring.DemoTimerListener;

import helios;

import helios.examples.scoring.IdConfig;

using namespace helios::engine::state;
using namespace helios::engine::state::types;
using namespace helios::engine::state::listeners;
using namespace helios::engine::runtime::world;
using namespace helios::engine::mechanics::timing::types;
using namespace helios::engine::mechanics::timing::commands;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::types;

export namespace helios::examples::scoring {

    inline void installDemoTimeListeners(
        StateManager<GameState>& gameStateManager,
        StateManager<MatchState>& matchStateManager,
        TimerManager& timerManager
    ) {
        gameStateManager.addStateListener(
            std::make_unique<LambdaStateListener<GameState>>(
            [&timerManager](UpdateContext& updateContext,
                 const StateTransitionContext<GameState> transitionContext) -> void {

                    const auto current = timerManager.gameTimer(IdConfig::DemoTimerId);
                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();

                    bool unpause = (from == GameState::Paused && to == GameState::Running);
                    bool pause = (from == GameState::Running && to == GameState::Paused);

                    if (unpause) {
                        if (current->state() != TimerState::Paused) {
                            return;
                        }
                        auto context = TimerControlContext{IdConfig::DemoTimerId, TimerState::Running};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    } else if (pause) {
                        if (current->state() != TimerState::Running) {
                            return;
                        }
                        auto context = TimerControlContext{IdConfig::DemoTimerId, TimerState::Paused};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    }

            })
        );


        matchStateManager.addStateListener(
            std::make_unique<LambdaStateListener<MatchState>>(
            [&timerManager](UpdateContext& updateContext,
                 const StateTransitionContext<MatchState> transitionContext) -> void {

                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();
                    const auto transitionId = transitionContext.transitionId();

                    bool start = (transitionId == MatchStateTransitionId::PlayerSpawnRequest && to == MatchState::Playing);
                    bool stop = (to == MatchState::GameOver || (from == MatchState::Start && to == MatchState::Countdown));

                    if (start) {
                        auto context = TimerControlContext{IdConfig::DemoTimerId, TimerState::Running};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    } else if (stop) {
                        auto context = TimerControlContext{ IdConfig::DemoTimerId, TimerState::Cancelled};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    }

            })
        );
    }

}
