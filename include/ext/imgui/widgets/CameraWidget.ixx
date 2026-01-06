/**
 * @file CameraWidget.ixx
 * @brief ImGui widget for controlling and configuring camera parameters.
 */
module;

#include <memory>
#include <string>
#include <vector>
#include "imgui.h"

export module helios.ext.imgui.widgets.CameraWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.scene.Camera;
import helios.scene.CameraSceneNode;
import helios.core.spatial.Transform;
import helios.math.types;
import helios.math.utils;

export namespace helios::ext::imgui::widgets {

    /**
     * @class CameraWidget
     * @brief Debug widget for real-time camera parameter control and visualization.
     */
    class CameraWidget : public ImGuiWidget {

    private:
        struct CameraEntry {
            std::string name;
            helios::scene::CameraSceneNode* node = nullptr;

            helios::math::vec3f initialTranslation{0.0f, 0.0f, 5.0f};
            helios::math::vec3f initialScale{1.0f, 1.0f, 1.0f};
            helios::math::mat4f initialRotation = helios::math::mat4f::identity();

            float initialFovDegrees = 90.0f;
            float initialAspectRatio = 16.0f / 9.0f;
            float initialZNear = 0.1f;
            float initialZFar = 1000.0f;

            helios::math::vec3f initialLookAtTarget{0.0f, 0.0f, 0.0f};
            helios::math::vec3f initialUp{0.0f, 1.0f, 0.0f};
        };

        enum class LookAtSpace { Local, World };

        std::vector<CameraEntry> cameras_;
        int selectedCameraIndex_ = 0;

        // Temporary UI values
        helios::math::vec3f tempTranslation_{0.0f, 0.0f, 5.0f};
        helios::math::vec3f tempLookAtTarget_{0.0f, 0.0f, 0.0f};
        helios::math::vec3f tempUp_{0.0f, 1.0f, 0.0f};

        float tempFovDegrees_ = 90.0f;
        float tempAspectRatio_ = 16.0f / 9.0f;
        float tempZNear_ = 0.1f;
        float tempZFar_ = 1000.0f;

        // LookAt mode
        LookAtSpace lookAtSpace_ = LookAtSpace::Local;

        // Apply behavior toggles
        bool applyTranslationOnChange_ = true;
        bool applyLookAtOnChange_      = true;
        bool applyBothOnAnyChange_     = false; // if enabled, apply both whenever either changes

        [[nodiscard]] helios::scene::CameraSceneNode* getCurrentCameraNode() noexcept {
            if (cameras_.empty()) {
                return nullptr;
            }
            if (selectedCameraIndex_ < 0 || selectedCameraIndex_ >= static_cast<int>(cameras_.size())) {
                selectedCameraIndex_ = 0;
            }
            return cameras_[selectedCameraIndex_].node;
        }

        [[nodiscard]] helios::scene::Camera* getActiveCamera() noexcept {
            auto* node = getCurrentCameraNode();
            if (!node) {
                return nullptr;
            }
            return &(node->camera());
        }

        /**
         * Applies translation and/or lookAt depending on change flags and apply-mode toggles.
         */
        void applyTransformToNode(helios::scene::CameraSceneNode* node,
                                  bool translationChanged,
                                  bool lookAtChanged)
        {
            if (!node) return;

            bool applyTranslation = applyTranslationOnChange_ && translationChanged;
            bool applyLookAt      = applyLookAtOnChange_      && lookAtChanged;

            // Force both together if enabled
            if (applyBothOnAnyChange_ && (translationChanged || lookAtChanged)) {
                applyTranslation = true;
                applyLookAt = true;
            }

            if (applyTranslation) {
                node->setTranslation(tempTranslation_);
            }

            if (!applyLookAt) {
                return;
            }

            if (lookAtSpace_ == LookAtSpace::Local) {
                node->lookAtLocal(tempLookAtTarget_, tempUp_);
            } else {
                node->lookAt(tempLookAtTarget_, tempUp_);
            }
        }

        void syncTempValuesFromCamera() noexcept {
            auto* node = getCurrentCameraNode();
            if (!node) {
                return;
            }

            const auto& transform = node->localTransform();
            tempTranslation_ = transform.translation();

            const auto& cam = node->camera();
            tempFovDegrees_ = helios::math::degrees(cam.fovY());
            tempAspectRatio_ = cam.aspectRatio();
            tempZNear_ = cam.zNear();
            tempZFar_ = cam.zFar();
        }

        void captureInitialValues(CameraEntry& entry) noexcept {
            if (!entry.node) {
                return;
            }

            const auto& transform = entry.node->localTransform();
            entry.initialTranslation = transform.translation();
            entry.initialScale = transform.scaling();
            entry.initialRotation = transform.rotation();

            const auto& cam = entry.node->camera();
            entry.initialFovDegrees = helios::math::degrees(cam.fovY());
            entry.initialAspectRatio = cam.aspectRatio();
            entry.initialZNear = cam.zNear();
            entry.initialZFar = cam.zFar();
        }

        void resetToInitialValues() noexcept {
            if (cameras_.empty()) {
                return;
            }

            auto& entry = cameras_[selectedCameraIndex_];
            auto* node = entry.node;
            if (!node) {
                return;
            }

            tempTranslation_ = entry.initialTranslation;
            tempLookAtTarget_ = entry.initialLookAtTarget;
            tempUp_ = entry.initialUp;

            node->setTranslation(entry.initialTranslation);
            node->setRotation(entry.initialRotation);
            node->setScale(entry.initialScale);

            auto& cam = node->camera();
            cam.setPerspective(helios::math::radians(entry.initialFovDegrees),
                               entry.initialAspectRatio,
                               entry.initialZNear,
                               entry.initialZFar);

            // Apply lookAt immediately after reset if user wants to keep that behavior consistent
            // (Here we always do it because reset expects the camera to match initial pose.)
            if (lookAtSpace_ == LookAtSpace::Local) {
                node->lookAtLocal(tempLookAtTarget_, tempUp_);
            } else {
                node->lookAt(tempLookAtTarget_, tempUp_);
            }

            syncTempValuesFromCamera();
        }

    public:
        CameraWidget() = default;

        void addCameraSceneNode(const std::string& name, helios::scene::CameraSceneNode* node) {
            CameraEntry entry{name, node};
            captureInitialValues(entry);

            // store current temp defaults as initial values for look-at
            entry.initialLookAtTarget = tempLookAtTarget_;
            entry.initialUp = tempUp_;

            cameras_.push_back(entry);

            if (cameras_.size() == 1) {
                syncTempValuesFromCamera();
            }
        }

        void clearCameras() noexcept {
            cameras_.clear();
            selectedCameraIndex_ = 0;
        }

        void draw() override {
            ImGui::SetNextWindowSize(ImVec2(320, 620), ImGuiCond_FirstUseEver);

            if (!ImGui::Begin("Camera Control", nullptr, ImGuiWindowFlags_NoCollapse)) {
                ImGui::End();
                return;
            }

            if (cameras_.empty()) {
                ImGui::TextDisabled("No cameras registered.");
                ImGui::End();
                return;
            }

            // Camera selection
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
            auto* cameraPtr = getActiveCamera();
            if (!node || !cameraPtr) {
                ImGui::End();
                return;
            }
            auto& camera = *cameraPtr;

            ImGui::Separator();
            ImGui::Spacing();

            bool translationChanged = false;
            bool lookAtChanged = false;

            // === Transform ===
            ImGui::Text("Position (Translation)");
            translationChanged |= ImGui::DragFloat("X##Pos", &tempTranslation_[0], 0.1f, -100.0f, 100.0f, "%.2f");
            translationChanged |= ImGui::DragFloat("Y##Pos", &tempTranslation_[1], 0.1f, -100.0f, 100.0f, "%.2f");
            translationChanged |= ImGui::DragFloat("Z##Pos", &tempTranslation_[2], 0.1f, -100.0f, 100.0f, "%.2f");

            ImGui::Spacing();

            // LookAt space toggle
            ImGui::Text("LookAt Space");
            const int mode = (lookAtSpace_ == LookAtSpace::Local) ? 0 : 1;

            if (ImGui::RadioButton("Local (parent/model space)", mode == 0)) {
                lookAtSpace_ = LookAtSpace::Local;
                lookAtChanged = true;
            }
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mode == 1)) {
                lookAtSpace_ = LookAtSpace::World;
                lookAtChanged = true;
            }

            ImGui::Spacing();

            // Apply behavior toggles (horizontal layout for compactness)
            ImGui::Separator();
            ImGui::Text("Apply Behavior");
            ImGui::Checkbox("Translation", &applyTranslationOnChange_);
            ImGui::SameLine();
            ImGui::Checkbox("LookAt", &applyLookAtOnChange_);
            ImGui::SameLine();
            ImGui::Checkbox("Both", &applyBothOnAnyChange_);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("If enabled, changing either Position or LookAt re-applies BOTH.");
            }

            ImGui::Separator();
            ImGui::Spacing();

            // LookAt target controls
            ImGui::Text("Look-At Target");
            lookAtChanged |= ImGui::DragFloat("X##Target", &tempLookAtTarget_[0], 0.1f, -100.0f, 100.0f, "%.2f");
            lookAtChanged |= ImGui::DragFloat("Y##Target", &tempLookAtTarget_[1], 0.1f, -100.0f, 100.0f, "%.2f");
            lookAtChanged |= ImGui::DragFloat("Z##Target", &tempLookAtTarget_[2], 0.1f, -100.0f, 100.0f, "%.2f");

            ImGui::Spacing();

            ImGui::Text("Up Vector");
            lookAtChanged |= ImGui::DragFloat("X##Up", &tempUp_[0], 0.01f, -1.0f, 1.0f, "%.3f");
            lookAtChanged |= ImGui::DragFloat("Y##Up", &tempUp_[1], 0.01f, -1.0f, 1.0f, "%.3f");
            lookAtChanged |= ImGui::DragFloat("Z##Up", &tempUp_[2], 0.01f, -1.0f, 1.0f, "%.3f");

            ImGui::SameLine();
            if (ImGui::SmallButton("N##NormalizeUp")) {
                float len = tempUp_.length();
                if (len > 0.0001f) {
                    tempUp_ = tempUp_.normalize();
                    lookAtChanged = true;
                }
            }
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Normalize up vector");
            }

            // Apply changes respecting apply-mode toggles
            if (translationChanged || lookAtChanged) {
                applyTransformToNode(node, translationChanged, lookAtChanged);
            }

            ImGui::Separator();
            ImGui::Spacing();

            // === Projection ===
            ImGui::Text("Projection");

            bool projectionChanged = false;
            projectionChanged |= ImGui::SliderFloat("FOV", &tempFovDegrees_, 30.0f, 120.0f, "%.1f deg");
            projectionChanged |= ImGui::DragFloat("Near Plane", &tempZNear_, 0.01f, 0.001f, tempZFar_ - 0.01f, "%.3f");
            projectionChanged |= ImGui::DragFloat("Far Plane", &tempZFar_, 1.0f, tempZNear_ + 0.01f, 100000.0f, "%.1f");

            if (projectionChanged) {
                camera.setPerspective(helios::math::radians(tempFovDegrees_), tempAspectRatio_, tempZNear_, tempZFar_);
            }

            ImGui::Spacing();

            ImGui::Text("Aspect Ratio");
            if (ImGui::DragFloat("##Aspect", &tempAspectRatio_, 0.01f, 0.5f, 4.0f, "%.3f")) {
                camera.setAspectRatio(tempAspectRatio_);
            }

            if (ImGui::Button("16:9")) {
                tempAspectRatio_ = 16.0f / 9.0f;
                camera.setAspectRatio(tempAspectRatio_);
            }
            ImGui::SameLine();
            if (ImGui::Button("21:9")) {
                tempAspectRatio_ = 21.0f / 9.0f;
                camera.setAspectRatio(tempAspectRatio_);
            }
            ImGui::SameLine();
            if (ImGui::Button("32:9")) {
                tempAspectRatio_ = 32.0f / 9.0f;
                camera.setAspectRatio(tempAspectRatio_);
            }
            ImGui::SameLine();
            if (ImGui::Button("4:3")) {
                tempAspectRatio_ = 4.0f / 3.0f;
                camera.setAspectRatio(tempAspectRatio_);
            }
            ImGui::SameLine();
            if (ImGui::Button("1:1")) {
                tempAspectRatio_ = 1.0f;
                camera.setAspectRatio(tempAspectRatio_);
            }

            ImGui::Separator();
            ImGui::Spacing();

            // Quick view presets
            ImGui::Text("Quick View Presets");
            if (ImGui::Button("Front")) {
                tempTranslation_ = {0.0f, 0.0f, 5.0f};
                tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                tempUp_ = {0.0f, 1.0f, 0.0f};
                applyTransformToNode(node, true, true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Top")) {
                tempTranslation_ = {0.0f, 5.0f, 0.001f};
                tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                tempUp_ = {0.0f, 0.0f, -1.0f};
                applyTransformToNode(node, true, true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Side")) {
                tempTranslation_ = {5.0f, 0.0f, 0.0f};
                tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                tempUp_ = {0.0f, 1.0f, 0.0f};
                applyTransformToNode(node, true, true);
            }
            ImGui::SameLine();
            if (ImGui::Button("Iso")) {
                tempTranslation_ = {5.0f, 5.0f, 5.0f};
                tempLookAtTarget_ = {0.0f, 0.0f, 0.0f};
                tempUp_ = {0.0f, 1.0f, 0.0f};
                applyTransformToNode(node, true, true);
            }

            ImGui::Separator();

            helios::math::vec3f diff = tempTranslation_ - tempLookAtTarget_;
            float distance = diff.length();

            ImGui::TextDisabled("Pos: (%.1f, %.1f, %.1f) | Target: (%.1f, %.1f, %.1f)",
                                tempTranslation_[0], tempTranslation_[1], tempTranslation_[2],
                                tempLookAtTarget_[0], tempLookAtTarget_[1], tempLookAtTarget_[2]);
            ImGui::TextDisabled("Distance: %.2f | FOV: %.0f° | Z: [%.2f, %.0f]",
                                distance, tempFovDegrees_, tempZNear_, tempZFar_);

            ImGui::End();
        }
    };

} // namespace helios::ext::imgui::widgets
