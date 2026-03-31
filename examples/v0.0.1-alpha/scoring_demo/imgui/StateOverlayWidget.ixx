/**
 * @file StateOverlayWidget.ixx
 * @brief ImGui overlay widget displaying current GameState and MatchState transitions.
 */
module;

#include "imgui.h"

export module helios.examples.scoring.StateOverlayWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.engine.runtime.world.Session;
import helios.engine.mechanics.gamestate.types.GameState;
import helios.engine.mechanics.gamestate.types.GameStateTransitionId;
import helios.engine.mechanics.match.types.MatchState;
import helios.engine.mechanics.match.types.MatchStateTransitionId;
import helios.engine.state.Bindings;
import helios.engine.state.types;

using namespace helios::ext::imgui;
using namespace helios::engine::runtime::world;
using namespace helios::engine::mechanics::gamestate::types;
using namespace helios::engine::mechanics::match::types;
using namespace helios::engine::state::types;

export namespace helios::examples::scoring {

    /**
     * @brief Returns a string label for a GameState value.
     *
     * @param s The GameState value.
     *
     * @return Null-terminated string label.
     */
    constexpr const char* toString(const GameState s) noexcept {
        switch (s) {
            case GameState::Undefined: return "Undefined";
            case GameState::Booted:   return "Booted";
            case GameState::Title:     return "Title";
            case GameState::MatchReady:  return "MatchReady";
            case GameState::Paused:    return "Paused";
            case GameState::Running:   return "Running";
            default:                   return "?";
        }
    }

    /**
     * @brief Returns a string label for a GameStateTransitionId value.
     *
     * @param id The transition identifier.
     *
     * @return Null-terminated string label.
     */
    constexpr const char* toString(const GameStateTransitionId id) noexcept {
        switch (id) {
            case GameStateTransitionId::Undefined:         return "Undefined";
            case GameStateTransitionId::BootRequest:       return "BootRequest";
            case GameStateTransitionId::TitleRequest:      return "TitleRequest";
            case GameStateTransitionId::ReadyMatchRequest: return "ReadyMatchRequest";
            case GameStateTransitionId::StartMatchRequest: return "StartMatchRequest";
            case GameStateTransitionId::TogglePause:      return "TogglePause";
            default:                                      return "?";
        }
    }

    /**
     * @brief Returns a string label for a MatchState value.
     *
     * @param s The MatchState value.
     *
     * @return Null-terminated string label.
     */
    constexpr const char* toString(const MatchState s) noexcept {
        switch (s) {
            case MatchState::Undefined:   return "Undefined";
            case MatchState::Warmup:      return "Warmup";
            case MatchState::Start:       return "Start";
            case MatchState::Countdown:   return "Countdown";
            case MatchState::Playing:     return "Playing";
            case MatchState::PlayerDefeated: return "PlayerDefeated";
            case MatchState::GameOver:    return "GameOver";
            default:                      return "?";
        }
    }

    /**
     * @brief Returns a string label for a MatchStateTransitionId value.
     *
     * @param id The transition identifier.
     *
     * @return Null-terminated string label.
     */
    constexpr const char* toString(const MatchStateTransitionId id) noexcept {
        switch (id) {
            case MatchStateTransitionId::Undefined:         return "Undefined";
            case MatchStateTransitionId::WarmupRequest:     return "WarmupRequest";
            case MatchStateTransitionId::StartRequest:      return "StartRequest";
            case MatchStateTransitionId::GameOverRequest:      return "GameOverRequest";
            case MatchStateTransitionId::CountdownRequest:  return "CountdownRequest";
            case MatchStateTransitionId::QuitRequest:   return "QuitRequest";
            case MatchStateTransitionId::PlayerDied:        return "PlayerDied";
            case MatchStateTransitionId::PlayerSpawnRequest:  return "PlayerSpawnRequest";
            default:                                        return "?";
        }
    }

    /**
     * @brief ImGui debug overlay displaying GameState and MatchState transitions.
     *
     * @details Shows for each state type: from, transitionId, to.
     * Requires a pointer to the active Session, set via setSession().
     */
    class StateOverlayWidget : public ImGuiWidget {

        Session* session_ = nullptr;

    public:

        StateOverlayWidget() = default;

        /**
         * @brief Sets the session to read state from.
         *
         * @param session Pointer to the active session (must outlive this widget).
         */
        void setSession(Session* session) noexcept {
            session_ = session;
        }

        /**
         * @brief Renders the state overlay.
         */
        void draw() override {
            if (!session_) {
                return;
            }

            ImGui::SetNextWindowSize(ImVec2(320, 180), ImGuiCond_FirstUseEver);

            if (!ImGui::Begin("State Monitor", nullptr, ImGuiWindowFlags_NoCollapse)) {
                ImGui::End();
                return;
            }

            // --- GameState ---
            {
                const auto from         = session_->stateFrom<GameState>();
                const auto to           = session_->state<GameState>();
                const auto transitionId = session_->stateTransitionId<GameState>();

                ImGui::SeparatorText("GameState");
                ImGui::Columns(2, "gs_cols", true);
                ImGui::Text("From");          ImGui::NextColumn(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", toString(from));         ImGui::NextColumn();
                ImGui::Text("TransitionId");  ImGui::NextColumn(); ImGui::TextColored(ImVec4(0.5f, 1, 0.5f, 1), "%s", toString(transitionId)); ImGui::NextColumn();
                ImGui::Text("To");            ImGui::NextColumn(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%s", toString(to));           ImGui::NextColumn();
                ImGui::Columns(1);
            }

            ImGui::Spacing();

            // --- MatchState ---
            {
                const auto from         = session_->stateFrom<MatchState>();
                const auto to           = session_->state<MatchState>();
                const auto transitionId = session_->stateTransitionId<MatchState>();

                ImGui::SeparatorText("MatchState");
                ImGui::Columns(2, "ms_cols", true);
                ImGui::Text("From");          ImGui::NextColumn(); ImGui::TextColored(ImVec4(1, 1, 0, 1), "%s", toString(from));         ImGui::NextColumn();
                ImGui::Text("TransitionId");  ImGui::NextColumn(); ImGui::TextColored(ImVec4(0.5f, 1, 0.5f, 1), "%s", toString(transitionId)); ImGui::NextColumn();
                ImGui::Text("To");            ImGui::NextColumn(); ImGui::TextColored(ImVec4(0, 1, 1, 1), "%s", toString(to));           ImGui::NextColumn();
                ImGui::Columns(1);
            }

            ImGui::End();
        }
    };

}


