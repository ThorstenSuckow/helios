/**
 * @file GameFlowSystem.ixx
 * @brief System that drives the high-level game state flow.
 */
module;

export module helios.gameplay.gamestate.systems.GameFlowSystem;

import helios.gameplay.gamestate.types;

import helios.state.Bindings;

import helios.runtime.concepts;

import helios.state.commands;
import helios.state.types.StateTransitionRequest;

import helios.gameplay.gamestate.types;

import helios.ecs;
import helios.runtime.world.UpdateContext;
import helios.runtime.world.Session;

import helios.runtime.world.tags.SystemRole;

using namespace helios::state::commands;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::state::types;
using namespace helios::gameplay::gamestate;
using namespace helios::gameplay::gamestate::types;
export namespace helios::gameplay::gamestate::systems {

    /**
     * @brief System that drives automatic game state transitions.
     *
     * GameFlowSystem monitors the current GameState and issues StateCommands
     * to advance the state machine when appropriate. It tracks the previous
     * state and transition ID to avoid emitting duplicate commands.
     *
     * Typical flow: Undefined -> Start -> Title.
     *
     * @see GameState
     * @see GameStateTransitionId
     * @see StateCommand
     */
    template<typename TCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class GameFlowSystem {

        /**
         * @brief The previously observed game state.
         */
        GameState prevMatchState_ = GameState::Undefined;

        /**
         * @brief The previously observed state transition ID.
         */
        StateTransitionIdType<GameState> prevGameStateTransitionId_ = StateTransitionIdType<GameState>::Undefined;

    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;

        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Updates the game flow and emits state transition commands.
         *
         * Checks the current GameState from the session and compares it with
         * the previously observed state. If a transition is required, a
         * StateCommand is added to the command buffer.
         *
         * @param updateContext The update context providing session and command buffer access.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& buffer) noexcept {

            auto& session = updateContext.session();

            const auto gameState = session.state<GameState>();
            auto gameStateTransitionId = session.stateTransitionId<GameState>();

            // only return if the state is undefined.
            // in any other case, we allow to drive the state forward, even if
            // the previous state is the same as the current one, in case any guard
            // did veto the next state
            if (gameState == GameState::Undefined) {// && prevMatchState_ == gameState && prevGameStateTransitionId_ == gameStateTransitionId) {
                return;
            }

            prevMatchState_= gameState;
            prevGameStateTransitionId_ = gameStateTransitionId;

            switch (gameState) {
                case GameState::Booting: {
                    buffer.template add<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(gameState, GameStateTransitionId::BootRequest)
                    );
                    break;
                }

                case GameState::Booted: {
                    buffer.template add<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(gameState, GameStateTransitionId::WarmupRequest)
                    );
                    break;
                }


                case GameState::MatchReady: {
                    buffer.template add<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(gameState, GameStateTransitionId::StartMatchRequest)
                    );
                    break;
                }

                default:
                    break;
            }
        }
    };
}
