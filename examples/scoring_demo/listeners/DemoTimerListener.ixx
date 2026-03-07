
module;

#include <memory>

export module helios.examples.scoring.DemoTimerListener;

import helios;

import helios.examples.scoring.IdConfig;

export namespace helios::examples::scoring {

    using namespace helios::engine::mechanics::timing::types;
    using namespace helios::engine::mechanics::timing::commands;


    inline void installDemoTimeListeners(
        helios::engine::state::StateManager<helios::engine::mechanics::gamestate::types::GameState>& gameStateManager,
        helios::engine::state::StateManager<helios::engine::mechanics::match::types::MatchState>& matchStateManager
    ) {
        gameStateManager.addStateListener(
            std::make_unique<helios::engine::state::listeners::LambdaStateListener<helios::engine::mechanics::gamestate::types::GameState>>(
            [](helios::engine::runtime::world::UpdateContext& updateContext,
                 const helios::engine::state::types::StateTransitionContext<helios::engine::mechanics::gamestate::types::GameState> transitionContext)->void {

                    using namespace helios::engine::mechanics::gamestate::types;
                    using namespace helios::engine::mechanics::match::types;

                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();
                    const auto transitionId = transitionContext.transitionId();

                    bool unpause = (from == GameState::Paused && to == GameState::Running);
                    bool pause = (from == GameState::Running && to == GameState::Paused);

                    if (unpause) {
                        auto context = TimerControlContext{TimerState::Started, IdConfig::DemoTimerId};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    } else if (pause) {
                        auto context = TimerControlContext{TimerState::Paused, IdConfig::DemoTimerId};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    }

            })
        );


        matchStateManager.addStateListener(
            std::make_unique<helios::engine::state::listeners::LambdaStateListener<helios::engine::mechanics::match::types::MatchState>>(
            [](helios::engine::runtime::world::UpdateContext& updateContext,
                 const helios::engine::state::types::StateTransitionContext<helios::engine::mechanics::match::types::MatchState> transitionContext)->void {

                using namespace helios::engine::mechanics::gamestate::types;
                    using namespace helios::engine::mechanics::match::types;

                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();
                    const auto transitionId = transitionContext.transitionId();

                    bool start = (transitionId == MatchStateTransitionId::StartRequested && to == MatchState::Playing);
                    bool stop = (to == MatchState::GameOver);

                    if (start) {
                        auto context = TimerControlContext{TimerState::Started, IdConfig::DemoTimerId};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    } else if (stop) {
                        auto context = TimerControlContext{TimerState::Stopped, IdConfig::DemoTimerId};
                        updateContext.queueCommand<TimerControlCommand>(context);
                    }

            })
        );
    }

}

