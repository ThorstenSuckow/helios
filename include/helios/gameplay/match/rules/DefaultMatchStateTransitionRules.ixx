/**
 * @file DefaultMatchStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.gameplay.match.rules.DefaultMatchStateTransitionRules;


import helios.engine.state.Bindings;
import helios.engine.state.types;
import helios.gameplay.match.types;
import helios.gameplay.match.rules.guards;

using namespace helios::engine::state::types;
using namespace helios::gameplay::match::types;
using namespace helios::gameplay::match::rules::guards;

export namespace helios::gameplay::match::rules {

    /**
     * @brief Provides the default set of match state transition rules.
     */
    class DefaultMatchStateTransitionRules {

        static constexpr StateTransitionRule<MatchState> rules_[] = {

            StateTransitionRule<MatchState>(
                MatchState::Undefined,
                MatchStateTransitionId::WarmupRequest,
                MatchState::Warmup
            ),

            StateTransitionRule<MatchState>(
                MatchState::Warmup,
                MatchStateTransitionId::StartRequest,
                MatchState::Start
            ),

            StateTransitionRule<MatchState>(
                MatchState::Start,
                MatchStateTransitionId::CountdownRequest,
                MatchState::Countdown,
                &DefaultMatchStateTransitionGuards::isPlayerActive
            ),


            StateTransitionRule<MatchState>(
                MatchState::Countdown,
                MatchStateTransitionId::PlayerSpawnRequest,
                MatchState::Playing
            ),

            StateTransitionRule<MatchState>(
                MatchState::Playing,
                MatchStateTransitionId::PlayerDied,
                MatchState::PlayerDefeated
            ),

            StateTransitionRule<MatchState>(
               MatchState::PlayerDefeated,
               MatchStateTransitionId::CountdownRequest,
               MatchState::Countdown,
                &DefaultMatchStateTransitionGuards::hasLifeLeft
            ),


            StateTransitionRule<MatchState>(
               MatchState::PlayerDefeated,
               MatchStateTransitionId::GameOverRequest,
               MatchState::GameOver,
                &DefaultMatchStateTransitionGuards::hasNoLifeLeft
           ),

            StateTransitionRule<MatchState>(
                MatchState::GameOver,
                MatchStateTransitionId::WarmupRequest,
                MatchState::Warmup
            ),

            StateTransitionRule<MatchState>(
                MatchState::GameOver,
                MatchStateTransitionId::StartRequest,
                MatchState::Start
            ),


            // cancel states
            StateTransitionRule<MatchState>(
                MatchState::Countdown,
                MatchStateTransitionId::StartRequest,
                MatchState::Start
            ),
            StateTransitionRule<MatchState>(
                MatchState::Playing,
                MatchStateTransitionId::StartRequest,
                MatchState::Start
            ),
            StateTransitionRule<MatchState>(
                MatchState::PlayerDefeated,
                MatchStateTransitionId::StartRequest,
                MatchState::Start
            ),
            StateTransitionRule<MatchState>(
                MatchState::Playing,
                MatchStateTransitionId::WarmupRequest,
                MatchState::Warmup
            ),
            StateTransitionRule<MatchState>(
                MatchState::Countdown,
                MatchStateTransitionId::WarmupRequest,
                MatchState::Warmup
            ),
            StateTransitionRule<MatchState>(
                MatchState::PlayerDefeated,
                MatchStateTransitionId::WarmupRequest,
                MatchState::Warmup
            ),

            StateTransitionRule<MatchState>(
                MatchState::Playing,
                MatchStateTransitionId::GameOverRequest,
                MatchState::GameOver
            ),




        };

    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const StateTransitionRule<MatchState>> rules() {
            return rules_;
        }

    };


}

