module;

#include <memory>

export module helios.examples.scoring.UiActionCommandPolicy;

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

    class UiActionCommandPolicy {


    public:

        static void applyPolicies(helios::engine::modules::ui::UiActionCommandManager& uiActionCommandManager) {


            uiActionCommandManager.addPolicy(
                helios::engine::core::data::ActionId{"continueGame"},
                [](
                    helios::engine::runtime::world::UpdateContext& updateContext,
                    const helios::engine::modules::ui::commands::UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
                      helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::gamestate::types::GameState>(
                            helios::engine::mechanics::gamestate::types::GameState::Paused,
                            helios::engine::mechanics::gamestate::types::GameStateTransitionId::TogglePause
                        )
                    );
                }
            ).addPolicy(
                helios::engine::core::data::ActionId{"quitGame"},
                [](
                    helios::engine::runtime::world::UpdateContext& updateContext,
                    const helios::engine::modules::ui::commands::UiActionCommand& actionCommand) noexcept -> void {

                    updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::match::types::MatchState>>(
                        helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::match::types::MatchState>(
                            helios::engine::mechanics::match::types::MatchState::Playing,
                            helios::engine::mechanics::match::types::MatchStateTransitionId::QuitRequested
                        )
                    );


                }
            ).addPolicy(
                helios::engine::core::data::ActionId{"restartGame"},
                [](
                    helios::engine::runtime::world::UpdateContext& updateContext,
                    const helios::engine::modules::ui::commands::UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
                        helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::gamestate::types::GameState>(
                            helios::engine::mechanics::gamestate::types::GameState::Paused,
                            helios::engine::mechanics::gamestate::types::GameStateTransitionId::RestartRequested
                        )
                    );
                }
                ).addPolicy(
                    helios::engine::core::data::ActionId{"gameOverRetry"},
                    [](
                        helios::engine::runtime::world::UpdateContext& updateContext,
                        const helios::engine::modules::ui::commands::UiActionCommand& actionCommand) noexcept -> void {
                        updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
                        helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::gamestate::types::GameState>(
                                helios::engine::mechanics::gamestate::types::GameState::Running,
                                helios::engine::mechanics::gamestate::types::GameStateTransitionId::RestartRequested
                            )
                        );
                    }
                ).addPolicy(
                    helios::engine::core::data::ActionId{"gameOverQuitGame"},
                    [](
                        helios::engine::runtime::world::UpdateContext& updateContext,
                        const helios::engine::modules::ui::commands::UiActionCommand& actionCommand) noexcept -> void {
                        updateContext.commandBuffer().add<helios::engine::state::commands::StateCommand<helios::engine::mechanics::gamestate::types::GameState>>(
                        helios::engine::state::types::StateTransitionRequest<helios::engine::mechanics::gamestate::types::GameState>(
                                helios::engine::mechanics::gamestate::types::GameState::Running,
                                helios::engine::mechanics::gamestate::types::GameStateTransitionId::TitleRequested
                            )
                        );
                    }
                );


        }



    };




}