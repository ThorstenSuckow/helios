/**
 * @file DefaultMatchStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.match.rules.DefaultMatchStateTransitionRules;

import helios.engine.mechanics.match.types;
import helios.engine.mechanics.match.rules.guards;


export namespace helios::engine::mechanics::match::rules {

    using namespace helios::engine::mechanics::match::types;
    using namespace helios::engine::mechanics::match::rules::guards;

    /**
     * @brief Provides the default set of match state transition rules.
     */
    class DefaultMatchStateTransitionRules {

        static constexpr MatchStateTransitionRule rules_[] = {

            MatchStateTransitionRule(
                MatchState::Undefined,
                MatchStateTransitionId::WarmupRequested,
                MatchState::Warmup,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
                MatchState::Warmup,
                MatchStateTransitionId::PlayerSpawnRequested,
                MatchState::PlayerSpawn,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
                MatchState::PlayerSpawn,
                MatchStateTransitionId::IntroRequested,
                MatchState::Intro,
                MatchStateTransitionType::Standard,
                &DefaultMatchStateTransitionGuards::isPlayerActive
            ),

            MatchStateTransitionRule(
                MatchState::PlayerSpawn,
                MatchStateTransitionId::CountdownRequested,
                MatchState::Countdown,
                MatchStateTransitionType::Standard,
                &DefaultMatchStateTransitionGuards::isPlayerActive
            ),

            MatchStateTransitionRule(
               MatchState::PlayerSpawn,
               MatchStateTransitionId::StartRequested,
               MatchState::Playing,
               MatchStateTransitionType::Standard,
               &DefaultMatchStateTransitionGuards::isPlayerActive
           ),


            MatchStateTransitionRule(
                MatchState::Intro,
                MatchStateTransitionId::CountdownRequested,
                MatchState::Countdown,
                MatchStateTransitionType::Standard
            ),


            MatchStateTransitionRule(
                MatchState::Countdown,
                MatchStateTransitionId::StartRequested,
                MatchState::Playing,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
                MatchState::Playing,
                MatchStateTransitionId::PlayerDied,
                MatchState::PlayerDeath,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
               MatchState::PlayerDeath,
               MatchStateTransitionId::PlayerSpawnRequested,
               MatchState::PlayerSpawn,
               MatchStateTransitionType::Standard,
                &DefaultMatchStateTransitionGuards::isPlayerInactive
           ),

            MatchStateTransitionRule(
               MatchState::Playing,
               MatchStateTransitionId::QuitRequested,
               MatchState::PlayerDeath,
               MatchStateTransitionType::Standard
           ),


            MatchStateTransitionRule(
               MatchState::PlayerDeath,
               MatchStateTransitionId::QuitRequested,
               MatchState::GameOver,
               MatchStateTransitionType::Standard,
               &DefaultMatchStateTransitionGuards::isPlayerInactive
           ),

            MatchStateTransitionRule(
        MatchState::GameOver,
           MatchStateTransitionId::RestartRequested,
           MatchState::Warmup,
           MatchStateTransitionType::Standard,
            &DefaultMatchStateTransitionGuards::isPlayerInactive
        ),

        MatchStateTransitionRule(
        MatchState::GameOver,
        MatchStateTransitionId::QuitRequested,
        MatchState::Undefined,
        MatchStateTransitionType::Standard
        )




        };

    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const MatchStateTransitionRule> rules() {
            return rules_;
        }

    };


}

