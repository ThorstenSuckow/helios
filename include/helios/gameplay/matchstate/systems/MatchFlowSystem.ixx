/**
 * @file MatchFlowSystem.ixx
 * @brief System that drives the match state flow automatically.
 */
module;

export module helios.gameplay.matchstate.systems.MatchFlowSystem;

import helios.gameplay.matchstate.types;

import helios.state.Bindings;


import helios.gameplay.matchstate.types.MatchState;
import helios.gameplay.matchstate.types.MatchStateTransitionId;

import helios.state.commands;
import helios.state.types.StateTransitionRequest;

import helios.state.types;
import helios.gameplay.gamestate.types;

import helios.ecs;
import helios.runtime;
import helios.runtime.concepts;

using namespace helios::state::commands;
using namespace helios::state::types;
using namespace helios::gameplay::gamestate;
using namespace helios::gameplay::matchstate::types;
using namespace helios::runtime::world;
using namespace helios::runtime::messaging::command::concepts;
using namespace helios::runtime::messaging::command;

import helios.runtime.world.tags.SystemRole;

export namespace helios::gameplay::matchstate::systems {

    /**
     * @brief Automatically advances the match through its state phases.
     *
     * This system checks the current match state and issues appropriate
     * transition commands to progress through: Undefined -> Warmup ->
     * PlayerSpawn -> Playing.
     */
    template<typename TCommandBuffer>
    requires IsCommandBufferLike<TCommandBuffer>
    class MatchFlowSystem {

        MatchState prevMatchState_ = MatchState::Undefined;
        StateTransitionIdType<MatchState> prevMatchStateTransitionId_ = StateTransitionIdType<MatchState>::Undefined;


    public:

        using EngineRoleTag = helios::runtime::tags::SystemRole;
        using CommandBuffer_type = TCommandBuffer;

        /**
         * @brief Processes match state and issues transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::runtime::world::UpdateContext& updateContext, TCommandBuffer& cmdBuffer) noexcept {

            auto& session = updateContext.session();

            const auto matchState = session.state<MatchState>();
            auto matchStateTransitionId = session.stateTransitionId<MatchState>();

            if (matchState != MatchState::Undefined && prevMatchState_ == matchState && prevMatchStateTransitionId_ == matchStateTransitionId) {
                return;
            }

            prevMatchState_= matchState;
            prevMatchStateTransitionId_ = matchStateTransitionId;

            switch (matchState) {


                case MatchState::Undefined: {
                    cmdBuffer.template add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::WarmupRequest)
                    );
                    break;
                }

                case MatchState::Warmup: {
                    cmdBuffer.template add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::StartRequest)
                    );
                    break;
                }

                case MatchState::Start: {
                    cmdBuffer.template add<StateCommand<MatchState>>(
                        StateTransitionRequest<MatchState>(matchState, MatchStateTransitionId::CountdownRequest)
                    );
                    break;
                }




                default:
                    break;
            }
        }
    };
}
