/**
 * @file SpaceshipWidget.ixx
 * @brief ImGui widget for tuning spaceship physics parameters at runtime.
 */
module;

#include <string>
#include <vector>
#include "imgui.h"

export module helios.examples.spaceshipShooting.SpaceshipWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.examples.spaceshipShooting.Spaceship;

using namespace helios::ext::imgui;

export namespace helios::examples::spaceshipShooting {

    /**
     * @brief Debug widget for real-time spaceship physics parameter tuning.
     *
     * @details This widget provides sliders and controls for adjusting all
     * configurable physics parameters of a Spaceship entity, including
     * rotation speed, movement speed, acceleration and dampening factors.
     *
     * Multiple spaceships can be registered and selected via a dropdown.
     */
    class SpaceshipWidget : public ImGuiWidget {

    private:
        struct SpaceshipEntry {
            std::string name;
            helios::examples::spaceshipShooting::Spaceship* spaceship = nullptr;
        };

        std::vector<SpaceshipEntry> spaceships_;
        int selectedIndex_ = 0;

        [[nodiscard]] helios::examples::spaceshipShooting::Spaceship* getSelectedSpaceship() noexcept {
            if (spaceships_.empty()) {
                return nullptr;
            }
            if (selectedIndex_ < 0 || selectedIndex_ >= static_cast<int>(spaceships_.size())) {
                selectedIndex_ = 0;
            }
            return spaceships_[selectedIndex_].spaceship;
        }

    public:
        SpaceshipWidget() = default;

        /**
         * @brief Registers a spaceship for tuning in this widget.
         *
         * @param name Display name for the spaceship in the dropdown.
         * @param spaceship Pointer to the Spaceship instance to control.
         */
        void addSpaceship(const std::string& name, helios::examples::spaceshipShooting::Spaceship* spaceship) {
            spaceships_.push_back({name, spaceship});
        }

        /**
         * @brief Removes all registered spaceships.
         */
        void clearSpaceships() noexcept {
            spaceships_.clear();
            selectedIndex_ = 0;
        }

        /**
         * @brief Renders the spaceship physics tuning interface.
         */
        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(350, 480), ImGuiCond_FirstUseEver);

            if (!ImGui::Begin("Spaceship Physics", nullptr, ImGuiWindowFlags_NoCollapse)) {
                ImGui::End();
                return;
            }

            if (spaceships_.empty()) {
                ImGui::TextDisabled("No spaceships registered.");
                ImGui::End();
                return;
            }

            // Spaceship selection
            if (ImGui::BeginCombo("##Spaceship", spaceships_[selectedIndex_].name.c_str())) {
                for (int i = 0; i < static_cast<int>(spaceships_.size()); ++i) {
                    const bool isSelected = (selectedIndex_ == i);
                    if (ImGui::Selectable(spaceships_[i].name.c_str(), isSelected)) {
                        selectedIndex_ = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            ImGui::SameLine();
            auto* ship = getSelectedSpaceship();
            if (ImGui::Button("Reset") && ship) {
                ship->resetPhysicsToDefaults();
            }

            if (!ship) {
                ImGui::End();
                return;
            }

            ImGui::Separator();
            ImGui::Spacing();

            // ========================================
            // Movement Parameters
            // ========================================
            ImGui::Text("Movement");

            float movementSpeed = ship->movementSpeed();
            if (ImGui::SliderFloat("Max Speed", &movementSpeed, 1.0f, 100.0f, "%.1f m/s")) {
                ship->setMovementSpeed(movementSpeed);
            }

            float acceleration = ship->movementAcceleration();
            if (ImGui::SliderFloat("Acceleration", &acceleration, 1.0f, 100.0f, "%.1f m/s²")) {
                ship->setMovementAcceleration(acceleration);
            }

            float movementDampening = ship->movementDampening();
            if (ImGui::SliderFloat("Dampening##Mov", &movementDampening, 0.001f, 1.0f, "%.4f")) {
                ship->setMovementDampening(movementDampening);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Exponential decay when input stops.\nLower = faster stop.");
            }

            float movementThreshold = ship->movementSpeedThreshold();
            if (ImGui::SliderFloat("Stop Threshold##Mov", &movementThreshold, 0.01f, 1.0f, "%.3f")) {
                ship->setMovementSpeedThreshold(movementThreshold);
            }

            ImGui::Separator();
            ImGui::Spacing();

            // ========================================
            // Rotation Parameters
            // ========================================
            ImGui::Text("Rotation");

            float rotationSpeed = ship->rotationSpeed();
            if (ImGui::SliderFloat("Max Speed##Rot", &rotationSpeed, 60.0f, 1000.0f, "%.0f °/s")) {
                ship->setRotationSpeed(rotationSpeed);
            }

            float rotationDampening = ship->rotationDampening();
            if (ImGui::SliderFloat("Dampening##Rot", &rotationDampening, 0.00001f, 0.01f, "%.5f")) {
                ship->setRotationDampening(rotationDampening);
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Exponential decay when input stops.\nLower = faster stop.");
            }

            float rotationThreshold = ship->rotationSpeedThreshold();
            if (ImGui::SliderFloat("Stop Threshold##Rot", &rotationThreshold, 0.01f, 1.0f, "%.3f")) {
                ship->setRotationSpeedThreshold(rotationThreshold);
            }

            ImGui::Separator();
            ImGui::Spacing();


            // ========================================
            // Status Display
            // ========================================
            ImGui::Text("Status");

            const auto& pos = ship->position();
            const auto& vel = ship->velocity();
            float speedRatio = ship->speedRatio();
            float angle = ship->rotationAngle();

            ImGui::TextDisabled("Position: (%.1f, %.1f, %.1f)", pos[0], pos[1], pos[2]);
            ImGui::TextDisabled("Velocity: (%.2f, %.2f, %.2f)", vel[0], vel[1], vel[2]);
            ImGui::TextDisabled("Speed: %.1f%% | Angle: %.1f°", speedRatio * 100.0f, angle);

            // Speed bar
            ImGui::ProgressBar(speedRatio, ImVec2(-1, 0), "");

            ImGui::End();
        }
    };

} // namespace helios::ext::imgui::widgets

