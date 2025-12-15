/**
 * @file GamepadWidget.ixx
 * @brief ImGui widget for visualizing real-time gamepad input state.
 */
module;

#include <string>
#include <vector>
#include <format>
#include <cmath>
#include <algorithm> // for std::clamp

// External ImGui dependency
#include "imgui.h"

export module helios.ext.imgui.widgets.GamepadWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.input.InputManager;
import helios.input.gamepad.GamepadState;
import helios.input.types.Gamepad;
import helios.math.types;

export namespace helios::ext::imgui::widgets {

    /**
     * @brief A debug widget to visualize the state of a specific gamepad.
     *
     * @details This widget provides a comprehensive visual overview of a connected gamepad's state.
     * It includes:
     * - **Connection Status:** Live indicator if the gamepad is connected.
     * - **Stick Visualization:** 2D plot showing the position of left/right analog sticks within a circular boundary.
     * - **Triggers:** Progress bars visualizing the pressure level of left/right triggers.
     * - **Buttons:** Read-only checkboxes showing the current pressed state of all standard gamepad buttons.
     *
     * The widget allows switching between different Gamepad IDs (1-4) at runtime via a dropdown menu.
     *
     * @note This widget is not thread-safe. Use from the main/render thread only.
     */
    class GamepadWidget : public ImGuiWidget {

    private:
        /**
         * @brief Pointer to the InputManager used to query gamepad states.
         * Non-owning pointer; must be valid for the lifetime of this widget.
         */
        helios::input::InputManager* inputManager_ = nullptr;

        /**
         * @brief The currently selected index in the UI combo box (0-3).
         * Maps to Gamepad::ONE through Gamepad::FOUR.
         */
        int selectedGamepadIndex_ = 0;

        /**
         * @brief Helper to map an integer index (0-3) to a Gamepad enum ID.
         *
         * @param index The index selected in the UI.
         * @return The corresponding Gamepad enum value.
         */
        [[nodiscard]] helios::input::types::Gamepad indexToId(int index) noexcept {
            switch (index) {
                case 0: return helios::input::types::Gamepad::ONE;
                case 1: return helios::input::types::Gamepad::TWO;
                case 2: return helios::input::types::Gamepad::THREE;
                case 3: return helios::input::types::Gamepad::FOUR;
                default: return helios::input::types::Gamepad::ONE;
            }
        }

        /**
         * @brief Helper method to draw a visual representation of an analog stick.
         *
         * Draws a circle representing the boundary and a dot representing the stick's position.
         *
         * @param label The label to display below the visualizer.
         * @param value The normalized (x, y) vector of the stick axis.
         * @param radius The radius of the visualization circle in pixels.
         */
        void drawStickVisualizer(const char* label, const helios::math::vec2f& value, float radius) {
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImDrawList* draw_list = ImGui::GetWindowDrawList();

            float center_x = p.x + radius;
            float center_y = p.y + radius;

            // Draw Background Circle (Dark Grey)
            draw_list->AddCircleFilled(ImVec2(center_x, center_y), radius, IM_COL32(50, 50, 50, 255));
            // Draw Boundary (White)
            draw_list->AddCircle(ImVec2(center_x, center_y), radius, IM_COL32(255, 255, 255, 255));

            // Calculate Dot Position
            // ImGui coordinates: Y increases downwards.
            // Standard Gamepad Y: Up is usually positive (or negative depending on normalization,
            // but we visually want "Up" stick to be "Up" on screen).
            // Assuming GamepadState provides standard Cartesian (Up=+Y), we subtract Y for screen coords.
            float dot_x = center_x + (value[0] * radius);
            float dot_y = center_y - (value[1] * radius);

            // Draw Stick Position Dot (Green)
            draw_list->AddCircleFilled(ImVec2(dot_x, dot_y), 4.0f, IM_COL32(0, 255, 0, 255));

            // Advance cursor to reserve space for the drawing
            ImGui::Dummy(ImVec2(radius * 2, radius * 2));
            ImGui::Text("%s: (%.2f, %.2f)", label, value[0], value[1]);
        }

    public:

        /**
         * @brief Constructs the GamepadWidget.
         *
         * @param inputManager Pointer to the InputManager to query states from.
         * Pass nullptr to render an empty/disabled widget.
         */
        explicit GamepadWidget(helios::input::InputManager* inputManager)
            : inputManager_(inputManager) {}

        /**
         * @brief Renders the gamepad debug interface using ImGui.
         */
        void draw() override {
            if (!inputManager_) return;

            // Helper Lambda to draw read-only checkboxes.
            // Solves the "address of r-value" error (C2102) by creating a temporary l-value.
            auto DrawReadOnlyCheckbox = [](const char* label, bool isPressed) {
                ImGui::BeginDisabled(); // Visually gray out to indicate read-only
                bool tempValue = isPressed; // Create l-value copy
                ImGui::Checkbox(label, &tempValue); // Pass address of l-value
                ImGui::EndDisabled();
            };

            ImGui::SetNextWindowSize(ImVec2(450, 550), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Gamepad Debugger", nullptr, ImGuiWindowFlags_NoCollapse)) {

                // 1. Gamepad Selection
                const char* items[] = { "Gamepad 1", "Gamepad 2", "Gamepad 3", "Gamepad 4" };
                ImGui::Combo("Select ID", &selectedGamepadIndex_, items, IM_ARRAYSIZE(items));

                auto gamepadId = indexToId(selectedGamepadIndex_);

                // 2. Connection Status
                bool connected = inputManager_->isConnected(gamepadId);
                ImGui::SameLine();
                if (connected) {
                    ImGui::TextColored(ImVec4(0, 1, 0, 1), "Connected");
                } else {
                    ImGui::TextColored(ImVec4(1, 0, 0, 1), "Disconnected");
                }

                ImGui::Separator();

                if (connected) {
                    // Retrieve const reference to state.
                    const auto& state = inputManager_->gamepadState(gamepadId);

                    // Layout: 2 Columns for Sticks
                    ImGui::Columns(2, "sticks", false);

                    // --- Left Stick ---
                    drawStickVisualizer("L-Stick", state.left(), 40.0f);
                    DrawReadOnlyCheckbox("L-Thumb", state.buttonLeftThumb());

                    ImGui::NextColumn();

                    // --- Right Stick ---
                    drawStickVisualizer("R-Stick", state.right(), 40.0f);
                    DrawReadOnlyCheckbox("R-Thumb", state.buttonRightThumb());

                    ImGui::Columns(1);
                    ImGui::Separator();

                    // --- Triggers (Progress Bars) ---
                    ImGui::Text("Triggers");

                    ImGui::Text("L: %.2f", state.triggerLeft());
                    ImGui::SameLine();
                    // ProgressBar expects [0.0, 1.0]
                    ImGui::ProgressBar(state.triggerLeft(), ImVec2(100, 0), "");

                    ImGui::SameLine(220); // Spacing

                    ImGui::Text("R: %.2f", state.triggerRight());
                    ImGui::SameLine();
                    ImGui::ProgressBar(state.triggerRight(), ImVec2(100, 0), "");

                    ImGui::Separator();

                    // --- Buttons ---
                    // Layout: 2 Columns (D-Pad vs Face Buttons)
                    ImGui::Columns(2, "buttons", false);

                    ImGui::Text("D-Pad");
                    DrawReadOnlyCheckbox("Up", state.buttonDpadUp());
                    DrawReadOnlyCheckbox("Down", state.buttonDpadDown());
                    DrawReadOnlyCheckbox("Left", state.buttonDpadLeft());
                    DrawReadOnlyCheckbox("Right", state.buttonDpadRight());

                    ImGui::NextColumn();

                    ImGui::Text("Face Buttons");
                    DrawReadOnlyCheckbox("A", state.buttonA());
                    DrawReadOnlyCheckbox("B", state.buttonB());
                    DrawReadOnlyCheckbox("X", state.buttonX());
                    DrawReadOnlyCheckbox("Y", state.buttonY());

                    ImGui::Columns(1);
                    ImGui::Separator();

                    // --- Middle / Bumpers ---
                    ImGui::Text("Center & Bumpers");

                    // Use manual layout for horizontal arrangement
                    DrawReadOnlyCheckbox("LB", state.buttonLeftBumper());
                    ImGui::SameLine();
                    DrawReadOnlyCheckbox("Back", state.buttonBack());
                    ImGui::SameLine();
                    DrawReadOnlyCheckbox("Guide", state.buttonGuide());
                    ImGui::SameLine();
                    DrawReadOnlyCheckbox("Start", state.buttonStart());
                    ImGui::SameLine();
                    DrawReadOnlyCheckbox("RB", state.buttonRightBumper());
                }
                else {
                    ImGui::TextDisabled("No input data available. Please connect a controller.");
                }
            }
            ImGui::End();
        }
    };
}