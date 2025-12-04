/**
 * @file MainMenuWidget.ixx
 * @brief ImGui main menu bar widget with application settings.
 */
module;

#include <functional>
#include "imgui.h"

export module helios.ext.imgui.widgets.MainMenuWidget;

import helios.ext.imgui.ImGuiWidget;

export namespace helios::ext::imgui::widgets {

    /**
     * @class MainMenuWidget
     * @brief Main menu bar providing access to application settings.
     *
     * This widget renders a menu bar at the top of the viewport with options for:
     * - View settings (window transparency, docking)
     * - Debug tools (show/hide log console, demo window)
     * - Style presets
     */
    class MainMenuWidget : public ImGuiWidget {

    private:
        /**
         * @brief Window background transparency (0.0 - 1.0).
         */
        float windowAlpha_ = 0.85f;

        /**
         * @brief Child window background transparency.
         */
        float childAlpha_ = 0.0f;

        /**
         * @brief Whether to show the ImGui demo window.
         */
        bool showDemoWindow_ = false;

        /**
         * @brief Whether to show the style editor.
         */
        bool showStyleEditor_ = false;

        /**
         * @brief Whether docking is enabled.
         */
        bool dockingEnabled_ = true;

        /**
         * @brief Callback when docking setting changes.
         */
        std::function<void(bool)> onDockingChanged_;

        /**
         * @brief Applies the current transparency settings to ImGui style.
         */
        void applyTransparency() {
            ImGuiStyle& style = ImGui::GetStyle();
            style.Colors[ImGuiCol_WindowBg].w = windowAlpha_;
            style.Colors[ImGuiCol_ChildBg].w = childAlpha_;
            style.Colors[ImGuiCol_PopupBg].w = windowAlpha_ + 0.05f;
            style.Colors[ImGuiCol_TitleBg].w = windowAlpha_ + 0.05f;
            style.Colors[ImGuiCol_TitleBgActive].w = windowAlpha_ + 0.1f;
        }

    public:
        MainMenuWidget() = default;
        ~MainMenuWidget() override = default;

        /**
         * @brief Sets a callback for when docking is enabled/disabled.
         *
         * @param callback Function called with the new docking state.
         */
        void setDockingCallback(std::function<void(bool)> callback) {
            onDockingChanged_ = std::move(callback);
        }

        /**
         * @brief Sets the window transparency level.
         *
         * @param alpha Transparency value (0.0 = transparent, 1.0 = opaque).
         */
        void setWindowAlpha(float alpha) {
            windowAlpha_ = alpha;
            applyTransparency();
        }

        /**
         * @brief Returns the current window transparency level.
         */
        [[nodiscard]] float windowAlpha() const noexcept {
            return windowAlpha_;
        }

        /**
         * @brief Renders the main menu bar.
         */
        void draw() override {
            if (ImGui::BeginMainMenuBar()) {

                // === View Menu ===
                if (ImGui::BeginMenu("View")) {

                    // Transparency settings
                    if (ImGui::BeginMenu("Transparency")) {
                        if (ImGui::SliderFloat("Window", &windowAlpha_, 0.3f, 1.0f, "%.2f")) {
                            applyTransparency();
                        }
                        if (ImGui::SliderFloat("Child Areas", &childAlpha_, 0.0f, 1.0f, "%.2f")) {
                            applyTransparency();
                        }

                        ImGui::Separator();

                        // Presets
                        if (ImGui::MenuItem("Solid")) {
                            windowAlpha_ = 1.0f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                        }
                        if (ImGui::MenuItem("Semi-Transparent")) {
                            windowAlpha_ = 0.85f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                        }
                        if (ImGui::MenuItem("Transparent")) {
                            windowAlpha_ = 0.6f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                        }
                        if (ImGui::MenuItem("Glass")) {
                            windowAlpha_ = 0.4f;
                            childAlpha_ = 0.2f;
                            applyTransparency();
                        }

                        ImGui::EndMenu();
                    }

                    // Docking toggle
                    if (ImGui::MenuItem("Docking", nullptr, &dockingEnabled_)) {
                        if (onDockingChanged_) {
                            onDockingChanged_(dockingEnabled_);
                        }
                    }

                    ImGui::EndMenu();
                }

                // === Style Menu ===
                if (ImGui::BeginMenu("Style")) {
                    if (ImGui::MenuItem("Dark")) {
                        ImGui::StyleColorsDark();
                        applyTransparency();
                    }
                    if (ImGui::MenuItem("Light")) {
                        ImGui::StyleColorsLight();
                        applyTransparency();
                    }
                    if (ImGui::MenuItem("Classic")) {
                        ImGui::StyleColorsClassic();
                        applyTransparency();
                    }

                    ImGui::Separator();

                    ImGui::MenuItem("Style Editor...", nullptr, &showStyleEditor_);

                    ImGui::EndMenu();
                }

                ImGui::EndMainMenuBar();
            }

            // Render optional windows
            if (showDemoWindow_) {
                ImGui::ShowDemoWindow(&showDemoWindow_);
            }

            if (showStyleEditor_) {
                if (ImGui::Begin("Style Editor", &showStyleEditor_)) {
                    ImGui::ShowStyleEditor();
                }
                ImGui::End();
            }
        }
    };

}

