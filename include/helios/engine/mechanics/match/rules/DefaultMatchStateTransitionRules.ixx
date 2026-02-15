/**
 * @file DefaultMatchStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.match.rules.DefaultMatchStateTransitionRules;

import helios.engine.mechanics.match.types;

export namespace helios::engine::mechanics::match::rules {

    using namespace helios::engine::mechanics::match::types;

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
                MatchStateTransitionId::IntroRequested,
                MatchState::Intro,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
                MatchState::Intro,
                MatchStateTransitionId::CountdownRequested,
                MatchState::Countdown,
                MatchStateTransitionType::Standard
            ),

            MatchStateTransitionRule(
                MatchState::Countdown,
                MatchStateTransitionId::PlayerSpawnRequested,
                MatchState::PlayerSpawn,
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
                MatchStateTransitionId::StartRequested,
                MatchState::Playing,
                MatchStateTransitionType::Standard
            ),


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

