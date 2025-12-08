/**
 * @file CameraWidget.ixx
 * @brief ImGui widget for controlling and configuring camera parameters.
 */
module;

#include <memory>
#include <vector>
#include <string>
#include "imgui.h"

export module helios.ext.imgui.widgets.CameraWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.scene.Camera;
import helios.scene.CameraSceneNode;
import helios.scene.Transform;
import helios.math.types;
import helios.math.utils;

export namespace helios::ext::imgui::widgets {

    /**
     * @class CameraWidget
     * @brief Debug widget for real-time camera parameter control and visualization.
     *
     * Provides runtime configuration of camera position, orientation, projection parameters,
     * and view settings through a flat, user-friendly interface. Multiple cameras can be
     * registered and selected via a combo box.
     *
     * Camera transforms (position, rotation) are controlled via CameraSceneNode,
     * while projection settings (FOV, aspect, near/far) are set directly on the Camera.
     *
     * @note Not thread-safe. Use from the main/render thread only.
     */
    class CameraWidget : public ImGuiWidget {

    private:
        /**
         * @brief Stores references to registered camera nodes with display names and initial values.
         */
        struct CameraEntry {
            std::string name;
            helios::scene::CameraSceneNode* node;

            // Initial transform values for reset functionality
            helios::math::vec3f initialTranslation{0.0f, 0.0f, 5.0f};
            helios::math::vec3f initialScale{1.0f, 1.0f, 1.0f};
            helios::math::mat4f initialRotation = helios::math::mat4f::identity();

            // Initial projection values
            float initialFovDegrees = 90.0f;
            float initialAspectRatio = 16.0f / 9.0f;
            float initialZNear = 0.1f;
            float initialZFar = 1000.0f;

            // Initial lookAt target and up vector
            helios::math::vec3f initialLookAtTarget{0.0f, 0.0f, 0.0f};
            helios::math::vec3f initialUp{0.0f, 1.0f, 0.0f};
        };

        std::vector<CameraEntry> cameras_;

        int selectedCameraIndex_ = 0;

        // Temporary UI values for transform editing
        helios::math::vec3f tempTranslation_{0.0f, 0.0f, 5.0f};
        helios::math::vec3f tempLookAtTarget_{0.0f, 0.0f, 0.0f};
        helios::math::vec3f tempUp_{0.0f, 1.0f, 0.0f};

        // Temporary UI values for projection editing
        float tempFovDegrees_ = 90.0f;
        float tempAspectRatio_ = 16.0f / 9.0f;
        float tempZNear_ = 0.1f;
        float tempZFar_ = 1000.0f;

        /**
         * @brief Retrieves the currently selected camera node, if available.
         */
        [[nodiscard]] helios::scene::CameraSceneNode* getCurrentCameraNode() noexcept {
            if (cameras_.empty()) {
                return nullptr;
            }
            return cameras_[selectedCameraIndex_].node;
        }

        /**
         * @brief Locks and returns the camera from the active node.
         */
        [[nodiscard]] helios::scene::Camera* getActiveCamera() noexcept {
            auto* node = getCurrentCameraNode();
            if (!node) {
                return nullptr;
            }
            return &(node->camera());
        }

        /**
         * @brief Applies the current transform and lookAt to the camera node.
         */
        void applyTransformToNode(helios::scene::CameraSceneNode* node) {
            if (!node) return;

            // First set translation
            node->translate(tempTranslation_);

            // Then apply lookAt (uses translation internally)
            node->lookAt(tempLookAtTarget_, tempUp_);
        }

        /**
         * @brief Synchronizes temporary UI values with the currently selected camera node's state.
         */
        void syncTempValuesFromCamera() noexcept {
            auto* node = getCurrentCameraNode();
            if (!node) {
                return;
            }

            // Get transform values from node
            const auto& transform = node->localTransform();
            tempTranslation_ = transform.translation();

            // Get projection values from camera (use reference, not copy)
            const auto& camera = node->camera();
            tempFovDegrees_ = helios::math::degrees(camera.fovY());
            tempAspectRatio_ = camera.aspectRatio();
            tempZNear_ = camera.zNear();
            tempZFar_ = camera.zFar();
        }

        /**
         * @brief Stores current camera/node values as initial values for reset.
         */
        void captureInitialValues(CameraEntry& entry) noexcept {
            if (!entry.node) {
                return;
            }

            // Capture transform
            const auto& transform = entry.node->localTransform();
            entry.initialTranslation = transform.translation();
            entry.initialScale = transform.scaling();
            entry.initialRotation = transform.rotation();

            // Capture projection (use reference, not copy)
            const auto& camera = entry.node->camera();

            entry.initialFovDegrees = helios::math::degrees(camera.fovY());
            entry.initialAspectRatio = camera.aspectRatio();
            entry.initialZNear = camera.zNear();
            entry.initialZFar = camera.zFar();
        }

        /**
         * @brief Resets the current camera node to its initial values.
         */
        void resetToInitialValues() noexcept {
            if (cameras_.empty()) {
                return;
            }

            auto& entry = cameras_[selectedCameraIndex_];
            auto* node = entry.node;
            if (!node) {
                return;
            }

            // Reset temp values
            tempTranslation_ = entry.initialTranslation;
            tempLookAtTarget_ = entry.initialLookAtTarget;
            tempUp_ = entry.initialUp;

            // Apply to node
            node->translate(entry.initialTranslation);
            node->rotate(entry.initialRotation);
            node->scale(entry.initialScale);

            // Reset projection via camera (use reference to modify actual camera)
            auto& camera = node->camera();
            camera.setPerspective(
                helios::math::radians(entry.initialFovDegrees),
                entry.initialAspectRatio,
                entry.initialZNear,
                entry.initialZFar
            );

            syncTempValuesFromCamera();
        }

    public:
        CameraWidget() = default;

        /**
         * @brief Registers a camera scene node with the widget and captures initial values for reset.
         *
         * @param name Display name for the camera (shown in combo box).
         * @param node Non-owning pointer to the CameraSceneNode instance. Must remain valid.
         */
        void addCameraSceneNode(const std::string& name, helios::scene::CameraSceneNode* node) {
            CameraEntry entry{name, node};
            captureInitialValues(entry);

            // Store current lookAt defaults as initial values
            entry.initialLookAtTarget = tempLookAtTarget_;
            entry.initialUp = tempUp_;

            cameras_.push_back(entry);

            if (cameras_.size() == 1) {
                syncTempValuesFromCamera();
            }
        }

        /**
         * @brief Removes all registered cameras from the widget.
         */
        void clearCameras() noexcept {
            cameras_.clear();
            selectedCameraIndex_ = 0;
        }

        /**
         * @brief Renders the camera control interface using ImGui.
         */
        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(320, 620), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("Camera Control", nullptr, ImGuiWindowFlags_NoCollapse)) {

                if (cameras_.empty()) {
                    ImGui::TextDisabled("No cameras registered.");
                    ImGui::End();
                    return;
                }

                // Camera Selection and Reset
                ImGui::PushItemWidth(-100);
                if (ImGui::BeginCombo("##Camera", cameras_[selectedCameraIndex_].name.c_str())) {
                    for (int i = 0; i < static_cast<int>(cameras_.size()); ++i) {
                        const bool isSelected = (selectedCameraIndex_ == i);
                        if (ImGui::Selectable(cameras_[i].name.c_str(), isSelected)) {
                            selectedCameraIndex_ = i;
                            syncTempValuesFromCamera();
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();

                ImGui::SameLine();
                if (ImGui::Button("Reset")) {
                    resetToInitialValues();
                }

                auto* node = getCurrentCameraNode();
                auto camera = getActiveCamera();
                if (!node || !camera) {
                    ImGui::End();
                    return;
                }

                ImGui::Separator();
                ImGui::Spacing();

                // Track if any transform value changed
                bool transformChanged = false;

                // === Transform Controls (via CameraSceneNode) ===
                ImGui::Text("Position (Translation)");
                transformChanged |= ImGui::DragFloat("X##Pos", &tempTranslation_[0], 0.1f, -100.0f, 100.0f, "%.2f");
                transformChanged |= ImGui::DragFloat("Y##Pos", &tempTranslation_[1], 0.1f, -100.0f, 100.0f, "%.2f");
                transformChanged |= ImGui::DragFloat("Z##Pos", &tempTranslation_[2], 0.1f, -100.0f, 100.0f, "%.2f");

                ImGui::Spacing();

                // Look-At Controls (via CameraSceneNode::lookAt)
                ImGui::Text("Look-At Target");
                transformChanged |= ImGui::DragFloat("X##Target", &tempLookAtTarget_[0], 0.1f, -100.0f, 100.0f, "%.2f");
                transformChanged |= ImGui::DragFloat("Y##Target", &tempLookAtTarget_[1], 0.1f, -100.0f, 100.0f, "%.2f");
                transformChanged |= ImGui::DragFloat("Z##Target", &tempLookAtTarget_[2], 0.1f, -100.0f, 100.0f, "%.2f");

                ImGui::Spacing();

                ImGui::Text("Up Vector");
                transformChanged |= ImGui::DragFloat("X##Up", &tempUp_[0], 0.01f, -1.0f, 1.0f, "%.3f");
                transformChanged |= ImGui::DragFloat("Y##Up", &tempUp_[1], 0.01f, -1.0f, 1.0f, "%.3f");
                transformChanged |= ImGui::DragFloat("Z##Up", &tempUp_[2], 0.01f, -1.0f, 1.0f, "%.3f");

                // Normalize up vector button
                ImGui::SameLine();
                if (ImGui::SmallButton("N##NormalizeUp")) {
                    float len = tempUp_.length();
                    if (len > 0.0001f) {
                        tempUp_ = tempUp_.normalize();
                        transformChanged = true;
                    }
                }
                if (ImGui::IsItemHovered()) {
                    ImGui::SetTooltip("Normalize up vector");
                }

                // Apply all transform changes together
                if (transformChanged) {
                    applyTransformToNode(node);
                }

                ImGui::Separator();
                ImGui::Spacing();

                // === Projection Controls (via Camera) ===
                ImGui::Text("Projection");

                bool projectionChanged = false;
                projectionChanged |= ImGui::SliderFloat("FOV", &tempFovDegrees_, 30.0f, 120.0f, "%.1f deg");
                projectionChanged |= ImGui::DragFloat("Near Plane", &tempZNear_, 0.01f, 0.001f, tempZFar_ - 0.01f, "%.3f");
                projectionChanged |= ImGui::DragFloat("Far Plane", &tempZFar_, 1.0f, tempZNear_ + 0.01f, 100000.0f, "%.1f");

                if (projectionChanged) {
                    camera->setPerspective(helios::math::radians(tempFovDegrees_), tempAspectRatio_, tempZNear_, tempZFar_);
                }

                ImGui::Spacing();

                // Aspect Ratio
                ImGui::Text("Aspect Ratio");
                if (ImGui::DragFloat("##Aspect", &tempAspectRatio_, 0.01f, 0.5f, 4.0f, "%.3f")) {
                    camera->setAspectRatio(tempAspectRatio_);
                }

                if (ImGui::Button("16:9")) {
                    tempAspectRatio_ = 16.0f / 9.0f;
                    camera->setAspectRatio(tempAspectRatio_);
                }
                ImGui::SameLine();
                if (ImGui::Button("21:9")) {
                    tempAspectRatio_ = 21.0f / 9.0f;
                    camera->setAspectRatio(tempAspectRatio_);
                }
                ImGui::SameLine();
                if (ImGui::Button("32:9")) {
                    tempAspectRatio_ = 32.0f / 9.0f;
                    camera->setAspectRatio(tempAspectRatio_);
                }
                ImGui::SameLine();
                if (ImGui::Button("4:3")) {
                    tempAspectRatio_ = 4.0f / 3.0f;
                    camera->setAspectRatio(tempAspectRatio_);
                }
                ImGui::SameLine();
                if (ImGui::Button("1:1")) {
                    tempAspectRatio_ = 1.0f;
                    camera->setAspectRatio(tempAspectRatio_);
                }

                ImGui::Separator();
                ImGui::Spacing();

                // Quick Position Presets
                ImGui::Text("Quick View Presets");
                if (ImGui::Button("Front")) {
                    tempTranslation_ = {0.0f, 0.0f, 5.0f};
                    tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                    tempUp_ = {0.0f, 1.0f, 0.0f};
                    applyTransformToNode(node);
                }
                ImGui::SameLine();
                if (ImGui::Button("Top")) {
                    tempTranslation_ = {0.0f, 5.0f, 0.001f};
                    tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                    tempUp_ = {0.0f, 0.0f, -1.0f};
                    applyTransformToNode(node);
                }
                ImGui::SameLine();
                if (ImGui::Button("Side")) {
                    tempTranslation_ = {5.0f, 0.0f, 0.0f};
                    tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                    tempUp_ = {0.0f, 1.0f, 0.0f};
                    applyTransformToNode(node);
                }
                ImGui::SameLine();
                if (ImGui::Button("Iso")) {
                    tempTranslation_ = {5.0f, 5.0f, 5.0f};
                    tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                    tempUp_ = {0.0f, 1.0f, 0.0f};
                    applyTransformToNode(node);
                }

                ImGui::Separator();

                // Debug Info
                helios::math::vec3f diff = tempTranslation_ - tempLookAtTarget_;
                float distance = diff.length();

                ImGui::TextDisabled("Pos: (%.1f, %.1f, %.1f) | Target: (%.1f, %.1f, %.1f)",
                    tempTranslation_[0], tempTranslation_[1], tempTranslation_[2],
                    tempLookAtTarget_[0], tempLookAtTarget_[1], tempLookAtTarget_[2]);
                ImGui::TextDisabled("Distance: %.2f | FOV: %.0f° | Z: [%.2f, %.0f]",
                    distance, tempFovDegrees_, tempZNear_, tempZFar_);
            }

            ImGui::End();
        }
    };

}

