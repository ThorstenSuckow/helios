/**
 * @file GamepadSettingsWidget.ixx
 * @brief ImGui widget for configuring gamepad input settings.
 */
module;

#include <string>
#include <format>
#include "imgui.h"

export module helios.ext.imgui.widgets.GamepadSettingsWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.input.InputAdapter;
import helios.input.types.Gamepad;
import helios.input.gamepad.GamepadSettings;

export namespace helios::ext::imgui::widgets {

    /**
     * @brief A configuration widget for adjusting gamepad input settings.
     *
     * This widget provides interactive controls for configuring per-gamepad settings
     * including deadzone thresholds and axis inversion options. Changes are applied
     * in real-time to the underlying InputAdapter's GamepadSettings.
     *
     * **Features:**
     * - **Gamepad Selection:** Switch between controllers (1-4) via dropdown.
     * - **Deadzone Sliders:** Adjust left/right stick deadzone thresholds (0.0 to 0.9).
     * - **Axis Inversion:** Toggle X/Y axis inversion for both sticks.
     * - **Visual Feedback:** Deadzone values displayed as percentage.
     *
     * @note Changes are applied immediately to the InputAdapter. No explicit save required.
     * @note This widget is not thread-safe. Use from the main/render thread only.
     *
     * @see GamepadSettings for the underlying configuration data.
     * @see GamepadWidget for real-time input visualization.
     */
    class GamepadSettingsWidget : public ImGuiWidget {

    private:
        /**
         * @brief Pointer to the InputAdapter used to access gamepad settings.
         * Non-owning pointer; must be valid for the lifetime of this widget.
         */
        helios::input::InputAdapter* inputAdapter_ = nullptr;

        /**
         * @brief The currently selected gamepad index in the UI combo box (0-3).
         */
        int selectedGamepadIndex_ = 0;

        /**
         * @brief Helper to map an integer index (0-3) to a Gamepad enum ID.
         *
         * @param index The index selected in the UI.
         *
         * @return The corresponding Gamepad enum value.
         */
        [[nodiscard]] static helios::input::types::Gamepad indexToId(int index) noexcept {
            switch (index) {
                case 0: return helios::input::types::Gamepad::ONE;
                case 1: return helios::input::types::Gamepad::TWO;
                case 2: return helios::input::types::Gamepad::THREE;
                case 3: return helios::input::types::Gamepad::FOUR;
                default: return helios::input::types::Gamepad::ONE;
            }
        }

    public:

        /**
         * @brief Constructs the GamepadSettingsWidget.
         *
         * @param inputAdapter Pointer to the InputAdapter to configure.
         *                     Pass nullptr to render a disabled widget.
         */
        explicit GamepadSettingsWidget(helios::input::InputAdapter* inputAdapter)
            : inputAdapter_(inputAdapter) {}

        /**
         * @brief Renders the gamepad settings configuration interface.
         */
        void draw() override {
            if (!inputAdapter_) return;

            ImGui::SetNextWindowSize(ImVec2(350, 320), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Gamepad Settings", nullptr, ImGuiWindowFlags_NoCollapse)) {

                // --- Gamepad Selection ---
                const char* items[] = { "Gamepad 1", "Gamepad 2", "Gamepad 3", "Gamepad 4" };
                ImGui::Combo("Controller", &selectedGamepadIndex_, items, IM_ARRAYSIZE(items));

                auto gamepadId = indexToId(selectedGamepadIndex_);
                auto& settings = inputAdapter_->gamepadSettings(gamepadId);

                ImGui::Separator();

                // --- Deadzone Configuration ---
                ImGui::Text("Deadzone Settings");
                ImGui::Spacing();

                // Left Stick Deadzone
                float leftDeadzone = settings.leftStickDeadzone();
                ImGui::Text("Left Stick");
                if (ImGui::SliderFloat("##LeftDeadzone", &leftDeadzone, 0.0f, 0.9f, "%.2f")) {
                    settings.setLeftStickDeadzone(leftDeadzone);
                }
                ImGui::SameLine();
                ImGui::TextDisabled("(%.0f%%)", leftDeadzone * 100.0f);

                // Right Stick Deadzone
                float rightDeadzone = settings.rightStickDeadzone();
                ImGui::Text("Right Stick");
                if (ImGui::SliderFloat("##RightDeadzone", &rightDeadzone, 0.0f, 0.9f, "%.2f")) {
                    settings.setRightStickDeadzone(rightDeadzone);
                }
                ImGui::SameLine();
                ImGui::TextDisabled("(%.0f%%)", rightDeadzone * 100.0f);

                ImGui::Separator();

                // --- Axis Inversion ---
                ImGui::Text("Axis Inversion");
                ImGui::Spacing();

                // Left Stick Inversion
                ImGui::Text("Left Stick:");
                ImGui::SameLine();

                bool invertLeftX = settings.invertLeftX();
                if (ImGui::Checkbox("Invert X##Left", &invertLeftX)) {
                    settings.setInvertLeftX(invertLeftX);
                }
                ImGui::SameLine();

                bool invertLeftY = settings.invertLeftY();
                if (ImGui::Checkbox("Invert Y##Left", &invertLeftY)) {
                    settings.setInvertLeftY(invertLeftY);
                }

                // Right Stick Inversion
                ImGui::Text("Right Stick:");
                ImGui::SameLine();

                bool invertRightX = settings.invertRightX();
                if (ImGui::Checkbox("Invert X##Right", &invertRightX)) {
                    settings.setInvertRightX(invertRightX);
                }
                ImGui::SameLine();

                bool invertRightY = settings.invertRightY();
                if (ImGui::Checkbox("Invert Y##Right", &invertRightY)) {
                    settings.setInvertRightY(invertRightY);
                }

                ImGui::Separator();

                // --- Reset Button ---
                if (ImGui::Button("Reset to Defaults")) {
                    settings.setLeftStickDeadzone(0.0f);
                    settings.setRightStickDeadzone(0.0f);
                    settings.setInvertLeftX(false);
                    settings.setInvertLeftY(false);
                    settings.setInvertRightX(false);
                    settings.setInvertRightY(false);
                }

                ImGui::SameLine();
                ImGui::TextDisabled("(?)");
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip(
                        "Deadzone: Input below this threshold is ignored.\n"
                        "Helps filter out stick drift when idle.\n\n"
                        "Invert: Reverses the axis direction.\n"
                        "Useful for flight-style controls."
                    );
                }
            }
            ImGui::End();
        }
    };

}

