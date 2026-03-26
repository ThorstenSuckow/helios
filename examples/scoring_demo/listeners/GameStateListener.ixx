/**
 * @file GameStateListener.ixx
 * @brief Game state listener installation for the scoring demo.
 */
module;

#include <cassert>
#include <memory>

export module helios.examples.scoring.GameStateListener;

import helios.engine.state.Bindings;
import helios;

import helios.examples.scoring.IdConfig;

using namespace helios::engine::state;
using namespace helios::engine::state::types;
using namespace helios::engine::state::listeners;
using namespace helios::engine::state::commands;
using namespace helios::engine::runtime::world;
using namespace helios::engine::mechanics::lifecycle::types;
using namespace helios::engine::mechanics::lifecycle::commands;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::mechanics::timing;
using namespace helios::engine::mechanics::timing::types;

export namespace helios::examples::scoring {

    /**
     * @brief Installs game state transition listeners.
     *
     * @param gameStateManager The game state manager to register listeners with.
     */
    inline void installGameStateListeners(
        StateManager<GameState>& gameStateManager,
        TimerManager& timerManager
    ) {
        gameStateManager.addStateListener(
            std::make_unique<LambdaStateListener<GameState>>(
            [&timerManager](UpdateContext& updateContext,
                 const StateTransitionContext<GameState> transitionContext) -> void {

                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();
                    const auto transitionId = transitionContext.transitionId();

                    bool reset = (from == GameState::Title && to == GameState::MatchReady)   ||
                                 (from == GameState::Paused && to == GameState::Title) ||
                                 (from == GameState::Paused && to == GameState::MatchReady) ||
                                 (from == GameState::Running && to == GameState::MatchReady) ||
                                 (from == GameState::Running && to == GameState::Title);

                    if (reset) {
                        updateContext.queueCommand<WorldLifecycleCommand>(WorldLifecycleAction::Reset);
                    }


                if ((from == GameState::Paused || from == GameState::Running) &&
                    (transitionId == GameStateTransitionId::TitleRequest ||
                     transitionId == GameStateTransitionId::ReadyMatchRequest)) {
                    updateContext.queueCommand<StateCommand<MatchState>>(
                       StateTransitionRequest<MatchState>(
                           updateContext.session().state<MatchState>(),
                           MatchStateTransitionId::WarmupRequest
                       )
                    );
                }

            })
        );
    }

}