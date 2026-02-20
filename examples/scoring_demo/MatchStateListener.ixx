module;

#include <memory>

export module helios.examples.scoring.MatchStateListener;

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

    class MatchStateListener {


    public:

        static void installListeners(helios::engine::state::StateManager<helios::engine::mechanics::match::types::MatchState>& matchStateManager) {

        matchStateManager.addStateListener(
                std::make_unique<helios::engine::state::listeners::LambdaStateListener<helios::engine::mechanics::match::types::MatchState>>(
                [](helios::engine::runtime::world::UpdateContext& updateContext,
                        const helios::engine::mechanics::match::types::MatchState from)->void {

                },
                [](helios::engine::runtime::world::UpdateContext& updateContext,
                     const helios::engine::state::types::StateTransitionContext<helios::engine::mechanics::match::types::MatchState> transitionContext)->void {

                        if (transitionContext.to() == helios::engine::mechanics::match::types::MatchState::PlayerDeath &&
                            transitionContext.transitionId() == helios::engine::mechanics::match::types::MatchStateTransitionId::QuitRequested) {

                            updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
                                helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::gamestate::types::GameState>(
                                    helios::engine::mechanics::gamestate::types::GameState::Paused,
                                    helios::engine::mechanics::gamestate::types::GameStateTransitionId::TogglePause
                                )
                            );
                        }
                },
                [](helios::engine::runtime::world::UpdateContext& updateContext,
                    const helios::engine::mechanics::match::types::MatchState to)->void {
                        if (to == helios::engine::mechanics::match::types::MatchState::PlayerSpawn) {
                            if (auto player = updateContext.gameWorld().find(updateContext.session().playerEntityHandle())) {
                                if (player) player->setActive(true);
                            }
                        }
                        if (to == helios::engine::mechanics::match::types::MatchState::PlayerDeath || to == helios::engine::mechanics::match::types::MatchState::GameOver) {
                            if (auto player = updateContext.gameWorld().find(updateContext.session().playerEntityHandle())) {
                                if (player) player->setActive(false);
                            }
                        }
                })
            );


        }



    };




}