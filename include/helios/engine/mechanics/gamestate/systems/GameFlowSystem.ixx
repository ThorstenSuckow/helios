/**
 * @file GameFlowSystem.ixx
 * @brief System that drives the high-level game state flow.
 */
module;

export module helios.engine.mechanics.gamestate.systems.GameFlowSystem;

import helios.engine.mechanics.gamestate.types;

import helios.engine.state.Bindings;
import helios.engine.runtime.messaging.command.EngineCommandBuffer;

import helios.engine.state.commands;
import helios.engine.state.types.StateTransitionRequest;

import helios.engine.mechanics.gamestate.types;

import helios.engine.ecs;
import helios.engine.runtime;

import helios.engine.common.tags.SystemRole;

export namespace helios::engine::mechanics::gamestate::systems {

    using namespace helios::engine::state::commands;
    using namespace helios::engine::state::types;
    using namespace helios::engine::mechanics::gamestate;
    using namespace helios::engine::mechanics::gamestate::types;

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

        using EngineRoleTag = helios::engine::common::tags::SystemRole;

        /**
         * @brief Updates the game flow and emits state transition commands.
         *
         * Checks the current GameState from the session and compares it with
         * the previously observed state. If a transition is required, a
         * StateCommand is added to the command buffer.
         *
         * @param updateContext The update context providing session and command buffer access.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept {

            auto& session = updateContext.session();

            const auto gameState = session.state<GameState>();
            auto gameStateTransitionId = session.stateTransitionId<GameState>();

            if (gameState != GameState::Undefined && prevMatchState_ == gameState && prevGameStateTransitionId_ == gameStateTransitionId) {
                return;
            }

            prevMatchState_= gameState;
            prevGameStateTransitionId_ = gameStateTransitionId;

            switch (gameState) {

                case GameState::Undefined: {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(gameState, GameStateTransitionId::StartRequested)
                    );
                    break;
                }

                case GameState::Start: {
                    updateContext.queueCommand<StateCommand<GameState>>(
                        StateTransitionRequest<GameState>(gameState, GameStateTransitionId::TitleRequested)
                    );
                    break;
                }


                default:
                    break;
            }
        }
    };
}
