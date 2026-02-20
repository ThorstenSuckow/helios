/**
 * @file UiActionCommandPolicy.ixx
 * @brief UI action command policy registration for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.UiActionCommandPolicy;

import helios;

export namespace helios::examples::scoring {

    /**
     * @brief Registers UI action command policies for menu interactions.
     *
     * @param uiActionCommandManager The manager to register policies with.
     */
    inline void applyUiActionCommandPolicies(
        helios::engine::modules::ui::UiActionCommandManager& uiActionCommandManager
    ) {
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

}

