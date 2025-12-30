/**
 * @file SpaceshipWidget.ixx
 * @brief ImGui widget for tuning Move2DComponent and HeadingComponent physics parameters at runtime.
 */
module;

#include <string>
#include <vector>
#include "imgui.h"

export module helios.examples.spaceshipShooting.SpaceshipWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.engine.game.GameObject;
import helios.engine.game.components.physics.Move2DComponent;
import helios.engine.game.components.physics.HeadingComponent;

using namespace helios::ext::imgui;

export namespace helios::examples::spaceshipShooting {

    /**
     * @brief Debug widget for real-time physics parameter tuning.
     *
     * @details This widget provides sliders and controls for adjusting all
     * configurable physics parameters of a GameObject's Move2DComponent (movement)
     * and HeadingComponent (rotation).
     *
     * Multiple GameObjects can be registered and selected via a dropdown.
     */
    class SpaceshipWidget : public ImGuiWidget {

    private:
        struct GameObjectEntry {
            std::string name;
            helios::engine::game::GameObject* gameObject = nullptr;
        };

        std::vector<GameObjectEntry> gameObjects_;
        int selectedIndex_ = 0;

        [[nodiscard]] helios::engine::game::components::physics::Move2DComponent* getSelectedMove2D() noexcept {
            if (gameObjects_.empty()) {
                return nullptr;
            }
            if (selectedIndex_ < 0 || selectedIndex_ >= static_cast<int>(gameObjects_.size())) {
                selectedIndex_ = 0;
            }
            auto* go = gameObjects_[selectedIndex_].gameObject;
            return go ? go->get<helios::engine::game::components::physics::Move2DComponent>() : nullptr;
        }

        [[nodiscard]] helios::engine::game::components::physics::HeadingComponent* getSelectedHeading() noexcept {
            if (gameObjects_.empty()) {
                return nullptr;
            }
            if (selectedIndex_ < 0 || selectedIndex_ >= static_cast<int>(gameObjects_.size())) {
                selectedIndex_ = 0;
            }
            auto* go = gameObjects_[selectedIndex_].gameObject;
            return go ? go->get<helios::engine::game::components::physics::HeadingComponent>() : nullptr;
        }

    public:
        SpaceshipWidget() = default;

        /**
         * @brief Registers a GameObject for tuning in this widget.
         *
         * @param name Display name for the object in the dropdown.
         * @param gameObject Pointer to the GameObject instance.
         */
        void addGameObject(const std::string& name, helios::engine::game::GameObject* gameObject) {
            gameObjects_.push_back({name, gameObject});
        }

        /**
         * @brief Removes all registered GameObjects.
         */
        void clearGameObjects() noexcept {
            gameObjects_.clear();
            selectedIndex_ = 0;
        }

        /**
         * @brief Renders the physics tuning interface.
         */
        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(350, 480), ImGuiCond_FirstUseEver);

            if (!ImGui::Begin("Spaceship Physics", nullptr, ImGuiWindowFlags_NoCollapse)) {
                ImGui::End();
                return;
            }

            if (gameObjects_.empty()) {
                ImGui::TextDisabled("No GameObjects registered.");
                ImGui::End();
                return;
            }

            // GameObject selection
            if (ImGui::BeginCombo("##GameObject", gameObjects_[selectedIndex_].name.c_str())) {
                for (int i = 0; i < static_cast<int>(gameObjects_.size()); ++i) {
                    const bool isSelected = (selectedIndex_ == i);
                    if (ImGui::Selectable(gameObjects_[i].name.c_str(), isSelected)) {
                        selectedIndex_ = i;
                    }
                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }

            auto* move2D = getSelectedMove2D();
            auto* heading = getSelectedHeading();

            ImGui::SameLine();
            if (ImGui::Button("Reset")) {
                if (move2D) move2D->resetToDefaults();
                if (heading) heading->resetToDefaults();
            }

            if (!move2D && !heading) {
                ImGui::TextDisabled("Selected object has no Move2DComponent or HeadingComponent.");
                ImGui::End();
                return;
            }

            ImGui::Separator();
            ImGui::Spacing();

            // ========================================
            // Movement Parameters
            // ========================================
            if (move2D) {
                ImGui::Text("Movement");

                float movementSpeed = move2D->movementSpeed();
                if (ImGui::SliderFloat("Max Speed", &movementSpeed, 1.0f, 100.0f, "%.1f m/s")) {
                    move2D->setMovementSpeed(movementSpeed);
                }

                float acceleration = move2D->movementAcceleration();
                if (ImGui::SliderFloat("Acceleration", &acceleration, 1.0f, 100.0f, "%.1f m/s^2")) {
                    move2D->setMovementAcceleration(acceleration);
                }

                float movementDampening = move2D->movementDampening();
                if (ImGui::SliderFloat("Dampening##Mov", &movementDampening, 0.001f, 1.0f, "%.4f")) {
                    move2D->setMovementDampening(movementDampening);
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Exponential decay when input stops.\nLower = faster stop.");
                }

                float movementThreshold = move2D->movementSpeedThreshold();
                if (ImGui::SliderFloat("Stop Threshold##Mov", &movementThreshold, 0.01f, 1.0f, "%.3f")) {
                    move2D->setMovementSpeedThreshold(movementThreshold);
                }

                ImGui::Separator();
                ImGui::Spacing();
            }

            // ========================================
            // Rotation Parameters (HeadingComponent)
            // ========================================
            if (heading) {
                ImGui::Text("Rotation");

                float rotationSpeed = heading->rotationSpeed();
                if (ImGui::SliderFloat("Max Speed##Rot", &rotationSpeed, 60.0f, 1000.0f, "%.0f deg/s")) {
                    heading->setRotationSpeed(rotationSpeed);
                }

                float rotationDampening = heading->rotationDampening();
                if (ImGui::SliderFloat("Dampening##Rot", &rotationDampening, 0.00001f, 0.01f, "%.5f")) {
                    heading->setRotationDampening(rotationDampening);
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Exponential decay when input stops.\nLower = faster stop.");
                }

                float rotationThreshold = heading->rotationSpeedThreshold();
                if (ImGui::SliderFloat("Stop Threshold##Rot", &rotationThreshold, 0.01f, 1.0f, "%.3f")) {
                    heading->setRotationSpeedThreshold(rotationThreshold);
                }

                ImGui::Separator();
                ImGui::Spacing();
            }

            // ========================================
            // Status Display
            // ========================================
            ImGui::Text("Status");

            if (move2D) {
                const auto& vel = move2D->velocity();
                float speedRatio = move2D->speedRatio();
                ImGui::TextDisabled("Velocity: (%.2f, %.2f, %.2f)", vel[0], vel[1], vel[2]);
                ImGui::TextDisabled("Speed: %.1f%%", speedRatio * 100.0f);
                ImGui::ProgressBar(speedRatio, ImVec2(-1, 0), "");
            }

            if (heading) {
                float angle = heading->currentRotationAngle();
                ImGui::TextDisabled("Heading Angle: %.1f deg", angle);
            }

            ImGui::End();
        }
    };

} // namespace helios::examples::spaceshipShooting
