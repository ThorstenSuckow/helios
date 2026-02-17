/**
 * @file MatchFlowSystem.ixx
 * @brief System that drives the match state flow automatically.
 */
module;

export module helios.engine.mechanics.match.systems.MatchFlowSystem;

import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.commands;

import helios.engine.mechanics.gamestate.types;

import helios.engine.ecs;
import helios.engine.runtime;

export namespace helios::engine::mechanics::match::systems {

    using namespace helios::engine::mechanics::gamestate;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::commands;

    /**
     * @brief Automatically advances the match through its state phases.
     *
     * This system checks the current match state and issues appropriate
     * transition commands to progress through: Undefined -> Warmup ->
     * PlayerSpawn -> Playing.
     */
    class MatchFlowSystem : public helios::engine::ecs::System {

        MatchState prevMatchSate_ = MatchState::Undefined;
        MatchStateTransitionId prevMatchStateTransitionId_ = MatchStateTransitionId::Undefined;


    public:

        /**
         * @brief Processes match state and issues transition commands.
         *
         * @param updateContext The current update context.
         */
        void update(helios::engine::runtime::world::UpdateContext& updateContext) noexcept override {

            auto& session = updateContext.session();


            auto& commandBuffer = updateContext.commandBuffer();
            const auto matchState = session.matchState();
            const auto matchStateTransitionId = session.matchStateTransitionId();

            if (matchState != MatchState::Undefined && prevMatchSate_ == matchState && prevMatchStateTransitionId_ == matchStateTransitionId) {
                return;
            }

            prevMatchSate_= matchState;
            prevMatchStateTransitionId_ = matchStateTransitionId;

            switch (matchState) {

                case MatchState::Finished: {
                    commandBuffer.add<MatchStateCommand>(
                        MatchStateTransitionRequest(matchState, MatchStateTransitionId::WarmupRequested)
                    );
                    break;
                }

                case MatchState::Undefined: {
                    commandBuffer.add<MatchStateCommand>(
                        MatchStateTransitionRequest(matchState, MatchStateTransitionId::WarmupRequested)
                    );
                    break;
                }

                case MatchState::Warmup: {
                    commandBuffer.add<MatchStateCommand>(
                        MatchStateTransitionRequest(matchState, MatchStateTransitionId::PlayerSpawnRequested)
                    );
                    break;
                }

                case MatchState::PlayerSpawn: {
                    commandBuffer.add<MatchStateCommand>(
                        MatchStateTransitionRequest(matchState, MatchStateTransitionId::StartRequested)
                    );
                    break;
                }

                case MatchState::PlayerDeath: {
                    if (matchStateTransitionId == MatchStateTransitionId::QuitRequested) {
                        commandBuffer.add<MatchStateCommand>(
                            MatchStateTransitionRequest(MatchState::PlayerDeath, MatchStateTransitionId::QuitRequested)
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
