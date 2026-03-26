/**
 * @file UiActionCommandPolicy.ixx
 * @brief UI action command policy registration for the scoring demo.
 */
module;

#include <memory>

export module helios.examples.scoring.UiActionCommandPolicy;

import helios;

using namespace helios::engine::modules::ui;
using namespace helios::engine::modules::ui::widgets::types;
using namespace helios::engine::modules::ui::widgets::commands;
using namespace helios::engine::runtime::world;
using namespace helios::engine::state::commands;
using namespace helios::engine::state::types;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;

export namespace helios::examples::scoring {

    /**
     * @brief Registers UI action command policies for menu interactions.
     *
     * @param uiActionCommandManager The manager to register policies with.
     */
    inline void applyUiActionCommandPolicies(
        UiActionCommandManager& uiActionCommandManager
    ) {
        uiActionCommandManager.addPolicy(
                ActionId{"continueGame"},
                [](UpdateContext& updateContext, const UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(
                            GameState::Paused,
                            GameStateTransitionId::TogglePause
                        )
                    );
                }
            ).addPolicy(
                ActionId{"quitGame"},
                [](UpdateContext& updateContext, const UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(
                            GameState::Paused,
                            GameStateTransitionId::TitleRequest
                        )
                    );
                }
            ).addPolicy(
                ActionId{"restartGame"},
                [](UpdateContext& updateContext, const UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(
                            GameState::Paused,
                            GameStateTransitionId::ReadyMatchRequest
                        )
                    );
                }
            ).addPolicy(
                ActionId{"gameOverRetry"},
                [](UpdateContext& updateContext, const UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(
                            GameState::Running,
                            GameStateTransitionId::ReadyMatchRequest
                        )
                    );
                }
            ).addPolicy(
                ActionId{"gameOverQuitGame"},
                [](UpdateContext& updateContext, const UiActionCommand& actionCommand) noexcept -> void {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(
                            GameState::Running,
                            GameStateTransitionId::TitleRequest
                        )
                    );
                }
            );
    }

}
