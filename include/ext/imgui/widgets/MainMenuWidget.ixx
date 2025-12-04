/**
 * @file MainMenuWidget.ixx
 * @brief ImGui main menu bar widget with application settings.
 */
module;

#include <functional>
#include <string>
#include <cstring>
#include <cstdio>
#include "imgui.h"
#include "imgui_internal.h"

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
     *
     * Settings are automatically saved to and loaded from ImGui's imgui.ini file.
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
         * @brief Whether this is the first draw call (for initial setup).
         */
        bool firstDraw_ = true;

        /**
         * @brief Whether the settings handler has been registered.
         */
        bool handlerRegistered_ = false;

        /**
         * @brief Callback when docking setting changes.
         */
        std::function<void(bool)> onDockingChanged_;

        /**
         * @brief Static pointer to the current instance (for ImGui callbacks).
         */
        static inline MainMenuWidget* instance_ = nullptr;

        /**
         * @brief Applies the current transparency settings to ImGui style.
         */
        void applyTransparency() {
            ImGuiStyle& style = ImGui::GetStyle();
            style.Colors[ImGuiCol_WindowBg].w = windowAlpha_;
            style.Colors[ImGuiCol_ChildBg].w = childAlpha_;
            style.Colors[ImGuiCol_PopupBg].w = std::min(windowAlpha_ + 0.05f, 1.0f);
            style.Colors[ImGuiCol_TitleBg].w = std::min(windowAlpha_ + 0.05f, 1.0f);
            style.Colors[ImGuiCol_TitleBgActive].w = std::min(windowAlpha_ + 0.1f, 1.0f);
        }

        /**
         * @brief Registers the ImGui settings handler for persistence.
         */
        void registerSettingsHandler() {
            if (handlerRegistered_) return;

            instance_ = this;

            ImGuiSettingsHandler handler;
            handler.TypeName = "HeliosUserSettings";
            handler.TypeHash = ImHashStr("HeliosUserSettings");
            handler.ClearAllFn = nullptr;
            handler.ReadOpenFn = [](ImGuiContext*, ImGuiSettingsHandler*, const char* name) -> void* {
                return (std::strcmp(name, "Main") == 0) ? instance_ : nullptr;
            };
            handler.ReadLineFn = [](ImGuiContext*, ImGuiSettingsHandler*, void* entry, const char* line) {
                auto* widget = static_cast<MainMenuWidget*>(entry);
                float f;
                int i;
                if (std::sscanf(line, "WindowAlpha=%f", &f) == 1) { widget->windowAlpha_ = f; }
                else if (std::sscanf(line, "ChildAlpha=%f", &f) == 1) { widget->childAlpha_ = f; }
                else if (std::sscanf(line, "DockingEnabled=%d", &i) == 1) { widget->dockingEnabled_ = (i != 0); }
            };
            handler.ApplyAllFn = [](ImGuiContext*, ImGuiSettingsHandler*) {
                if (instance_) {
                    instance_->applyTransparency();
                }
            };
            handler.WriteAllFn = [](ImGuiContext*, ImGuiSettingsHandler* h, ImGuiTextBuffer* buf) {
                if (!instance_) return;
                buf->appendf("[%s][Main]\n", h->TypeName);
                buf->appendf("WindowAlpha=%.3f\n", instance_->windowAlpha_);
                buf->appendf("ChildAlpha=%.3f\n", instance_->childAlpha_);
                buf->appendf("DockingEnabled=%d\n", instance_->dockingEnabled_ ? 1 : 0);
                buf->append("\n");
            };

            ImGui::GetCurrentContext()->SettingsHandlers.push_back(handler);
            handlerRegistered_ = true;
        }

    public:
        /**
         * @brief Default constructor, registers settings handler with ImGui.
         */
        MainMenuWidget() {
            registerSettingsHandler();
        }

        ~MainMenuWidget() override {
            if (instance_ == this) {
                instance_ = nullptr;
            }
        }

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
            ImGui::MarkIniSettingsDirty();
        }

        /**
         * @brief Returns the current window transparency level.
         */
        [[nodiscard]] float windowAlpha() const noexcept {
            return windowAlpha_;
        }

        /**
         * @brief Returns whether docking is enabled.
         */
        [[nodiscard]] bool isDockingEnabled() const noexcept {
            return dockingEnabled_;
        }

        /**
         * @brief Renders the main menu bar.
         */
        void draw() override {
            // Apply settings on first draw
            if (firstDraw_) {
                applyTransparency();
                if (onDockingChanged_) {
                    onDockingChanged_(dockingEnabled_);
                }
                firstDraw_ = false;
            }

            if (ImGui::BeginMainMenuBar()) {

                // === View Menu ===
                if (ImGui::BeginMenu("View")) {

                    // Transparency settings
                    if (ImGui::BeginMenu("Transparency")) {
                        if (ImGui::SliderFloat("Window", &windowAlpha_, 0.3f, 1.0f, "%.2f")) {
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }
                        if (ImGui::SliderFloat("Child Areas", &childAlpha_, 0.0f, 1.0f, "%.2f")) {
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }

                        ImGui::Separator();

                        // Presets
                        if (ImGui::MenuItem("Solid")) {
                            windowAlpha_ = 1.0f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }
                        if (ImGui::MenuItem("Semi-Transparent")) {
                            windowAlpha_ = 0.85f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }
                        if (ImGui::MenuItem("Transparent")) {
                            windowAlpha_ = 0.6f;
                            childAlpha_ = 0.0f;
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }
                        if (ImGui::MenuItem("Glass")) {
                            windowAlpha_ = 0.4f;
                            childAlpha_ = 0.2f;
                            applyTransparency();
                            ImGui::MarkIniSettingsDirty();
                        }

                        ImGui::EndMenu();
                    }

                    // Docking toggle
                    if (ImGui::MenuItem("Docking", nullptr, &dockingEnabled_)) {
                        if (onDockingChanged_) {
                            onDockingChanged_(dockingEnabled_);
                        }
                        ImGui::MarkIniSettingsDirty();
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

                // === Debug Menu ===
                if (ImGui::BeginMenu("Debug")) {
                    ImGui::MenuItem("ImGui Demo Window", nullptr, &showDemoWindow_);
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

