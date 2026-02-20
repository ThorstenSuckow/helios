module;

#include <memory>

export module helios.examples.scoring.GameStateListener;

import helios;
import helios.ext;


import helios.examples.scoring.IdConfig;
import helios.examples.scoring.ArenaConfig;
import helios.examples.scoring.CollisionId;

export namespace helios::examples::scoring {

    using namespace helios::engine::runtime::world;
    using namespace helios::engine::builder::gameObject;
    using namespace helios::engine::ecs;
    using namespace helios::ext::opengl::rendering::shader;
    using namespace helios::ext::opengl::rendering;
    using namespace helios::ext::opengl;
    using namespace helios::scene;
    using namespace helios::rendering;

    class GameStateListener {


    public:

        static void installListeners(helios::engine::state::StateManager<helios::engine::mechanics::gamestate::types::GameState>& gameStateManager) {

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



    };




}