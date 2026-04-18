/**
 * @file GameStateInputResponseSystem.ixx
 * @brief System that handles input for game state transitions.
 */
module;


export module helios.gameplay.gamestate.systems.GameStateInputResponseSystem;

import helios.state;

import helios.state.Bindings;

import helios.runtime.concepts;

import helios.gameplay.gamestate.types;

import helios.input.types.Gamepad;

import helios.ecs;
import helios.runtime;


import helios.runtime.world.tags.SystemRole;

using namespace helios::input::types;
using namespace helios::state::types;
using namespace helios::state::commands;
using namespace helios::runtime::concepts;
using namespace helios::gameplay::gamestate::types;
using namespace helios::runtime::messaging::command;
export namespace helios::gameplay::gamestate::systems {



    /**
     * @brief Responds to gamepad input and issues game state commands.
     *
     * Listens for the Start button and triggers appropriate state transitions
     * based on the current game state (e.g., Title -> Running, Running -> Paused).
     */
    template<typename TCommandBuffer = NullCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class GameStateInputResponseSystem {



    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;

        /**
         * @brief Processes input and submits state transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext) noexcept {

            auto gameState = updateContext.session().state<GameState>();

            if (updateContext.inputSnapshot().gamepadState().isButtonPressed(GamepadInput::Start)) {

                switch (gameState) {
                    case GameState::Title:
                        updateContext.queueCommand<TCommandBuffer, StateCommand<GameState>>(
                            StateTransitionRequest<GameState>(GameState::Title, GameStateTransitionId::ReadyMatchRequest)
                        );
                        break;

                    case GameState::Running:
                        updateContext.queueCommand<TCommandBuffer, StateCommand<GameState>>(
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