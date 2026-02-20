/**
 * @file MatchFlowSystem.ixx
 * @brief System that drives the match state flow automatically.
 */
module;

export module helios.engine.mechanics.match.systems.MatchFlowSystem;

import helios.engine.mechanics.match.types;


import helios.engine.mechanics.match.types.MatchState;
import helios.engine.mechanics.match.types.MatchStateTransitionId;

import helios.engine.state.commands.StateCommand;
import helios.engine.state.types.StateTransitionRequest;

import helios.engine.state.types;
import helios.engine.mechanics.gamestate.types;

import helios.engine.ecs;
import helios.engine.runtime;

export namespace helios::engine::mechanics::match::systems {

    using namespace helios::engine::state::commands;
    using namespace helios::engine::state::types;
    using namespace helios::engine::mechanics::gamestate;
    using namespace helios::engine::mechanics::match::types;

    /**
     * @brief Automatically advances the match through its state phases.
     *
     * This system checks the current match state and issues appropriate
     * transition commands to progress through: Undefined -> Warmup ->
     * PlayerSpawn -> Playing.
     */
    class MatchFlowSystem : public helios::engine::ecs::System {

        MatchState prevMatchState_ = MatchState::Undefined;
        StateTransitionIdType<MatchState> prevMatchStateTransitionId_ = StateTransitionIdType<MatchState>::Undefined;


    public:

        /**
         * @brief Processes match state and issues transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto& session = updateContext.session();


            auto& commandBuffer = updateContext.commandBuffer();
            const auto matchState = session.state<MatchState>();
            auto matchStateTransitionId = session.stateTransitionId<MatchState>();

            if (matchState != MatchState::Undefined && prevMatchState_ == matchState && prevMatchStateTransitionId_ == matchStateTransitionId) {
                return;
            }

            prevMatchState_= matchState;
            prevMatchStateTransitionId_ = matchStateTransitionId;

            switch (matchState) {

                case MatchState::Finished: {
                    commandBuffer.add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::WarmupRequested)
                    );
                    break;
                }

                case MatchState::Undefined: {
                    commandBuffer.add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::WarmupRequested)
                    );
                    break;
                }

                case MatchState::Warmup: {
                    commandBuffer.add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::PlayerSpawnRequested)
                    );
                    break;
                }

                case MatchState::PlayerSpawn: {
                    commandBuffer.add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::StartRequested)
                    );
                    break;
                }

                case MatchState::PlayerDeath: {
                    if (matchStateTransitionId == MatchStateTransitionId::QuitRequested) {
                        commandBuffer.add<StateCommand<MatchState>>(
                            StateTransitionRequest<MatchState>(MatchState::PlayerDeath, MatchStateTransitionId::QuitRequested)
                        );
                    }

                    break;
                }


                default:
                    break;
            }
        }
    };
}
