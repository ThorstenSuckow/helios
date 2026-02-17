/**
 * @file DefaultGameStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.gamestate.rules.DefaultGameStateTransitionRules;

import helios.engine.mechanics.gamestate.types;

export namespace helios::engine::mechanics::gamestate::rules {

    using namespace helios::engine::mechanics::gamestate::types;

    /**
     * @brief Provides the default set of game state transition rules.
     *
     * Defines standard transitions such as:
     * - Undefined -> Title (on TitleRequested)
     * - Title -> Running (on StartRequested)
     * - Running <-> Paused (on TogglePause)
     * - Paused -> Running (on RestartRequested)
     * - Paused -> Title (on QuitGameRequested)
     */
    class DefaultGameStateTransitionRules {

        static constexpr GameStateTransitionRule rules_[] = {

            GameStateTransitionRule(
                GameState::Undefined,
                GameStateTransitionId::TitleRequested,
                GameState::Title,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
                GameState::Title,
                GameStateTransitionId::StartRequested,
                GameState::Running,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
                GameState::Running,
                GameStateTransitionId::TogglePause,
                GameState::Paused,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
                GameState::Paused,
                GameStateTransitionId::TogglePause,
                GameState::Running,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
                GameState::Paused,
                GameStateTransitionId::RestartRequested,
                GameState::Running,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
                GameState::Running,
                GameStateTransitionId::RestartRequested,
                GameState::Running,
                GameStateTransitionType::Standard
            ),

            GameStateTransitionRule(
               GameState::Paused,
               GameStateTransitionId::RestartRequested,
               GameState::Running,
               GameStateTransitionType::Standard
           ),

            GameStateTransitionRule(
                GameState::Running,
                GameStateTransitionId::TitleRequested,
                GameState::Title,
                GameStateTransitionType::Standard
            ),

        };

    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const GameStateTransitionRule> rules() {
            return rules_;
        }

    };


}

