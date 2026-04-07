/**
 * @file DefaultGameStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>

export module helios.engine.mechanics.gamestate.rules.DefaultGameStateTransitionRules;


import helios.engine.state.Bindings;

import helios.engine.mechanics.gamestate.types;

import helios.engine.state.types.StateTransitionRule;
import helios.engine.state.types.StateTransitionId;


export namespace helios::engine::mechanics::gamestate::rules {

    using namespace helios::engine::mechanics::gamestate::types;
    using namespace helios::engine::state::types;

    /**
     * @brief Provides the default set of game state transition rules.
     */
    class DefaultGameStateTransitionRules {

        static constexpr StateTransitionRule<GameState> rules_[] = {

            StateTransitionRule<GameState>(
                GameState::Undefined,
                StateTransitionIdType<GameState>::BootRequest,
                GameState::Booting
            ),

            StateTransitionRule<GameState>(
                GameState::Booting,
                StateTransitionIdType<GameState>::BootRequest,
                GameState::Booted
            ),

            StateTransitionRule<GameState>(
                GameState::Booted,
                StateTransitionIdType<GameState>::TitleRequest,
                GameState::Title
            ),

            StateTransitionRule<GameState>(
                GameState::Title,
                GameStateTransitionId::ReadyMatchRequest,
                GameState::MatchReady
            ),

            StateTransitionRule<GameState>(
                GameState::MatchReady,
                GameStateTransitionId::StartMatchRequest,
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
                GameStateTransitionId::TitleRequest,
                GameState::Title
            ),

            StateTransitionRule<GameState>(
                GameState::Paused,
                GameStateTransitionId::ReadyMatchRequest,
                GameState::MatchReady
            ),

            StateTransitionRule<GameState>(
                GameState::Running,
                GameStateTransitionId::TitleRequest,
                GameState::Title
            ),

            StateTransitionRule<GameState>(
                GameState::Running,
                GameStateTransitionId::ReadyMatchRequest,
                GameState::MatchReady
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

