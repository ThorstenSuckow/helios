/**
 * @file DefaultGameStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.gamestate.rules.DefaultGameStateTransitionRules;

import helios.engine.mechanics.gamestate.types;

import helios.engine.state.types.StateTransitionRule;
import helios.engine.state.types.StateTransitionId;


export namespace helios::engine::mechanics::gamestate::rules {

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::state::types;

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

        static constexpr StateTransitionRule<GameState> rules_[] = {

            StateTransitionRule<GameState>(
                GameState::Undefined,
                StateTransitionIdType<GameState>::StartRequested,
                GameState::Start
            ),

            StateTransitionRule<GameState>(
                GameState::Start,
                StateTransitionIdType<GameState>::TitleRequested,
                GameState::Title
            ),
            StateTransitionRule<GameState>(

                GameState::Title,
                GameStateTransitionId::StartRequested,
                GameState::Running
            ),

            StateTransitionRule<GameState>(

                GameState::Running,
                GameStateTransitionId::TogglePause,
                GameState::Paused
            ),

            StateTransitionRule<GameState>(

                GameState::Paused,
                GameStateTransitionId::TogglePause,
                GameState::Running
                ),

            StateTransitionRule<GameState>(

                GameState::Paused,
                GameStateTransitionId::RestartRequested,
                GameState::Running
            ),

            StateTransitionRule<GameState>(

                GameState::Running,
                GameStateTransitionId::RestartRequested,
                GameState::Running
            ),

            StateTransitionRule<GameState>(

               GameState::Paused,
               GameStateTransitionId::RestartRequested,
               GameState::Running
           ),

            StateTransitionRule<GameState>(

                GameState::Running,
                GameStateTransitionId::TitleRequested,
                GameState::Title
            )

        };

    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const StateTransitionRule<GameState>> rules() {
            return rules_;
        }

    };


}

