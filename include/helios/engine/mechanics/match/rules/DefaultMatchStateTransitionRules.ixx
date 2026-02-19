/**
 * @file DefaultMatchStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.match.rules.DefaultMatchStateTransitionRules;

import helios.engine.state.types;
import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.rules.guards;


export namespace helios::engine::mechanics::match::rules {

    using namespace helios::engine::state::types;
    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::rules::guards;

    /**
     * @brief Provides the default set of match state transition rules.
     */
    class DefaultMatchStateTransitionRules {

        static constexpr StateTransitionRule<MatchState> rules_[] = {

            StateTransitionRule<MatchState>(
                MatchState::Undefined,
                MatchStateTransitionId::WarmupRequested,
                MatchState::Warmup
            ),

            StateTransitionRule<MatchState>(
                MatchState::Warmup,
                MatchStateTransitionId::PlayerSpawnRequested,
                MatchState::PlayerSpawn
            ),

            StateTransitionRule<MatchState>(
                MatchState::PlayerSpawn,
                MatchStateTransitionId::IntroRequested,
                MatchState::Intro,
                &DefaultMatchStateTransitionGuards::isPlayerActive
            ),

            StateTransitionRule<MatchState>(
                MatchState::PlayerSpawn,
                MatchStateTransitionId::CountdownRequested,
                MatchState::Countdown,
                &DefaultMatchStateTransitionGuards::isPlayerActive
            ),

            StateTransitionRule<MatchState>(
               MatchState::PlayerSpawn,
               MatchStateTransitionId::StartRequested,
               MatchState::Playing,
               &DefaultMatchStateTransitionGuards::isPlayerActive
           ),


            StateTransitionRule<MatchState>(
                MatchState::Intro,
                MatchStateTransitionId::CountdownRequested,
                MatchState::Countdown
            ),


            StateTransitionRule<MatchState>(
                MatchState::Countdown,
                MatchStateTransitionId::StartRequested,
                MatchState::Playing
            ),

            StateTransitionRule<MatchState>(
                MatchState::Playing,
                MatchStateTransitionId::PlayerDied,
                MatchState::PlayerDeath
            ),

            StateTransitionRule<MatchState>(
               MatchState::PlayerDeath,
               MatchStateTransitionId::PlayerSpawnRequested,
               MatchState::PlayerSpawn,
                &DefaultMatchStateTransitionGuards::isPlayerInactive
           ),

            StateTransitionRule<MatchState>(
               MatchState::Playing,
               MatchStateTransitionId::QuitRequested,
               MatchState::PlayerDeath
           ),


            StateTransitionRule<MatchState>(
               MatchState::PlayerDeath,
               MatchStateTransitionId::QuitRequested,
               MatchState::GameOver,
               &DefaultMatchStateTransitionGuards::isPlayerInactive
           ),

            StateTransitionRule<MatchState>(
        MatchState::GameOver,
           MatchStateTransitionId::RestartRequested,
           MatchState::Warmup,
            &DefaultMatchStateTransitionGuards::isPlayerInactive
        ),

            StateTransitionRule<MatchState>(
        MatchState::GameOver,
        MatchStateTransitionId::QuitRequested,
        MatchState::Undefined
        )




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

