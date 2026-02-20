/**
 * @file GameStateListener.ixx
 * @brief Game state listener installation for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.GameStateListener;

import helios;

export namespace helios::examples::scoring {

    /**
     * @brief Installs game state transition listeners.
     *
     * @param gameStateManager The game state manager to register listeners with.
     */
    inline void installGameStateListeners(
        helios::engine::state::StateManager<helios::engine::mechanics::gamestate::types::GameState>& gameStateManager
    ) {
        gameStateManager.addStateListener(
            std::make_unique<helios::engine::state::listeners::LambdaStateListener<helios::engine::mechanics::gamestate::types::GameState>>(
            [](helios::engine::runtime::world::UpdateContext& updateContext,
                    const helios::engine::mechanics::gamestate::types::GameState from)->void {
                        },
            [](helios::engine::runtime::world::UpdateContext& updateContext,
                 const helios::engine::state::types::StateTransitionContext<helios::engine::mechanics::gamestate::types::GameState> transitionContext)->void {

                    using namespace helios::engine::mechanics::gamestate::types;
                    using namespace helios::engine::mechanics::match::types;

                    const auto from = transitionContext.from();
                    const auto to = transitionContext.to();
                    const auto transitionId = transitionContext.transitionId();

                    bool reset = (from == GameState::Title && to == GameState::Running)   ||
                                 (from == GameState::Paused && to == GameState::Title)    ||
                                 ((from == GameState::Paused || from == GameState::Running) &&
                                     transitionId == GameStateTransitionId::RestartRequested &&
                                     to == GameState::Running) ||
                                 (from == GameState::Running && to == GameState::Title) ;

                    if (reset) {
                        updateContext.gameWorld().reset();
                    }

                    if (to == GameState::Title && transitionId== GameStateTransitionId::TitleRequested) {
                        updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<MatchState>>(
                            helios::engine::state::types::StateTransitionRequest<MatchState>(
                            MatchState::GameOver,
                                MatchStateTransitionId::RestartRequested
                            )
                        );
                    }
                    if (from == GameState::Running && transitionId == GameStateTransitionId::TitleRequested) {
                        updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<MatchState>>(
                            helios::engine::state::types::StateTransitionRequest<MatchState>(
                                MatchState::GameOver, MatchStateTransitionId::QuitRequested
                            )
                        );
                    }
                    if (from == GameState::Running && transitionId == GameStateTransitionId::RestartRequested) {
                        updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<MatchState>>(
                            helios::engine::state::types::StateTransitionRequest<MatchState>(
                            MatchState::GameOver,
                                MatchStateTransitionId::RestartRequested
                            )
                        );
                    }
            },
            [](helios::engine::runtime::world::UpdateContext& updateContext,
                 const helios::engine::mechanics::gamestate::types::GameState to)->void {
            })
        );
    }

}