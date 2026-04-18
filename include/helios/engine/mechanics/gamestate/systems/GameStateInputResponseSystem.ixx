/**
 * @file GameStateInputResponseSystem.ixx
 * @brief System that handles input for game state transitions.
 */
module;


export module helios.engine.mechanics.gamestate.systems.GameStateInputResponseSystem;

import helios.engine.state;

import helios.engine.state.Bindings;

import helios.engine.common.concepts;

import helios.engine.mechanics.gamestate.types;

import helios.input.types.Gamepad;

import helios.ecs;
import helios.engine.runtime;


import helios.engine.common.tags.SystemRole;

using namespace helios::input::types;
using namespace helios::engine::state::types;
using namespace helios::engine::state::commands;
using namespace helios::engine::common::concepts;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::runtime::messaging::command;
export namespace helios::engine::mechanics::gamestate::systems {



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

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Processes input and submits state transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

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