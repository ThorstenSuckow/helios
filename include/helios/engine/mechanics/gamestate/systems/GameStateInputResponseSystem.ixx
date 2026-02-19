/**
 * @file GameStateInputResponseSystem.ixx
 * @brief System that handles input for game state transitions.
 */
module;


export module helios.engine.mechanics.gamestate.systems.GameStateInputResponseSystem;

import helios.engine.state;

import helios.engine.mechanics.gamestate.types;

import helios.input.types.Gamepad;

import helios.engine.ecs;
import helios.engine.runtime;


export namespace helios::engine::mechanics::gamestate::systems {

    using namespace helios::input::types;
    using namespace helios::engine::state::types;
    using namespace helios::engine::state::commands;
    using namespace helios::engine::mechanics::gamestate::types;

    /**
     * @brief Responds to gamepad input and issues game state commands.
     *
     * Listens for the Start button and triggers appropriate state transitions
     * based on the current game state (e.g., Title -> Running, Running -> Paused).
     */
    class GameStateInputResponseSystem : public helios::engine::ecs::System {



    public:

        /**
         * @brief Processes input and submits state transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto gameState = updateContext.gameWorld().session().state<GameState>();

            if (updateContext.inputSnapshot().gamepadState().isButtonPressed(GamepadInput::Start)) {

                switch (gameState) {
                    case GameState::Title:
                        updateContext.commandBuffer().add<StateCommand<GameState>>(
                            StateTransitionRequest<GameState>(GameState::Title, GameStateTransitionId::StartRequested)
                        );
                        break;

                    case GameState::Running:
                        updateContext.commandBuffer().add<StateCommand<GameState>>(
                            StateTransitionRequest<GameState>(GameState::Running, GameStateTransitionId::TogglePause)
                        );
                        break;

                    default:
                        break;
                }
            }

        }

    };


}