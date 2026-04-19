/**
 * @file DefaultGameStateTransitionRules.ixx
 * @brief Default transition rules for the game state machine.
 */
module;

#include <span>
#include <array>
#include <cstddef>

export module helios.gameplay.gamestate.rules.DefaultGameStateTransitionRules;


import helios.state.Bindings;

import helios.runtime.world.UpdateContext;

import helios.gameplay.gamestate.types;
import helios.gameplay.gamestate.rules.guards;

import helios.state.types.StateTransitionRule;
import helios.state.types.StateTransitionId;
import helios.rendering.shader.types.ShaderHandle;

using namespace helios::state::types;
using namespace helios::rendering::shader::types;
using namespace helios::runtime::world;
using namespace helios::gameplay::gamestate::rules::guards;
export namespace helios::gameplay::gamestate::rules {

    using namespace helios::gameplay::gamestate::types;
    using namespace helios::state::types;

    /**
     * @brief Provides the default set of game state transition rules.
     */
    class DefaultGameStateTransitionRules {


        static constexpr auto rules_ = [] {
            std::array baseRules = std::to_array<StateTransitionRule<GameState>>({

            {GameState::Undefined,  GameStateTransitionId::BootRequest,       GameState::Booting},
            {GameState::Booting,    GameStateTransitionId::BootRequest,       GameState::Booted,
                &DefaultGameStateTransitionGuards::isPlatformInitialized},

            {GameState::Booted,     GameStateTransitionId::WarmupRequest,     GameState::Warmup,
                &DefaultGameStateTransitionGuards::isRuntimeInfrastructureReady},

            {GameState::Warmup,     GameStateTransitionId::WarmupDoneSignal,      GameState::Title},

            {GameState::Title,      GameStateTransitionId::ReadyMatchRequest, GameState::MatchReady},
            {GameState::MatchReady, GameStateTransitionId::StartMatchRequest, GameState::Running},
            {GameState::Running,    GameStateTransitionId::TogglePause,       GameState::Paused},
            {GameState::Paused,     GameStateTransitionId::TogglePause,       GameState::Running},
            {GameState::Paused,     GameStateTransitionId::TitleRequest,      GameState::Title},
            {GameState::Paused,     GameStateTransitionId::ReadyMatchRequest, GameState::MatchReady},
            {GameState::Running,    GameStateTransitionId::TitleRequest,      GameState::Title},
            {GameState::Running,    GameStateTransitionId::ReadyMatchRequest, GameState::MatchReady}
            });

            constexpr GameState shutdownable[] = {
                GameState::Title,
                GameState::MatchReady,
                GameState::Running,
                GameState::Warmup,
                GameState::Paused
            };

            std::array<StateTransitionRule<GameState>, baseRules.size() + std::size(shutdownable)> rules{};

            for (size_t i = 0; i < baseRules.size(); i++) {
                rules[i] = baseRules[i];
            }

            for (size_t i = 0; i < std::size(shutdownable); i++) {
                rules[baseRules.size() + i] = StateTransitionRule<GameState>{
                    shutdownable[i],
                    GameStateTransitionId::ShutdownRequest,
                    GameState::Shutdown
                };
            }

            return rules;
        }();



    public:

        /**
         * @brief Returns the default transition rules.
         *
         * @return A span of the predefined transition rules.
         */
        [[nodiscard]] static std::span<const StateTransitionRule<GameState>> rules() {
            return std::span{rules_};
        }

    };
    }

