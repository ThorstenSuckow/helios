/**
 * @file FpsWidget.ixx
 * @brief ImGui widget for displaying and configuring FPS metrics and frame pacing.
 */
module;

#include <vector>
#include <string>
#include "imgui.h"

export module helios.ext.imgui.widgets.FpsWidget;

import helios.ext.imgui.ImGuiWidget;
import helios.tooling.FpsMetrics;
import helios.engine.FramePacer;
import helios.engine.FrameStats;

export namespace helios::ext::imgui::widgets {

    /**
     * @class FpsWidget
     * @brief Debug widget for real-time FPS metrics and frame pacing configuration.
     *
     * Displays average FPS, frame time, work time, idle time, and frame history graph.
     * Optionally allows runtime configuration of frame pacing target FPS via buttons
     * and sliders (requires a `FramePacer` instance).
     *
     * @note This widget is not thread-safe. Use from the main/render thread only.
     */
    class FpsWidget : public ImGuiWidget {

    private:
        /**
         * @brief Pointer to an FpsMetrics instance used for tracking and displaying FPS metrics.
         *
         * This member provides direct access to real-time frame rate statistics,
         * including current FPS, average FPS, and frame timing data, which are displayed
         * in the widget. It is expected to be externally managed and supplied during widget
         * initialization.
         *
         * @note The pointer must remain valid throughout the lifetime of the FpsWidget instance.
         */
        helios::tooling::FpsMetrics* fpsMetrics_ = nullptr;

        /**
         * @brief Pointer to a `FramePacer` instance for managing frame pacing and target FPS.
         *
         * Provides frame pacing control to synchronize application updates with a
         * specified frame rate target, if set. When associated with a valid `FramePacer`,
         * it enables runtime configuration of frame timing and update intervals.
         *
         * @note This pointer must be initialized with a valid `FramePacer` instance or
         * set to `nullptr` if frame pacing is not required. Ensure the lifetime of the
         * referenced `FramePacer` extends beyond the usage of this pointer.
         */
        helios::engine::FramePacer* framePacer_ = nullptr;

        /**
         * @brief Stores the user-configured target frames per second (FPS) for frame pacing.
         *
         * Acts as the intermediate value for the target FPS, used for runtime adjustments
         * via buttons or sliders in the UI. This variable gets updated dynamically based on
         * user input and reflects the desired frame rate. A value of `0.0f` indicates an
         * uncapped frame rate.
         *
         * @note This variable is synchronized with the target FPS of the associated
         * `FramePacer` instance if provided.
         */
        float targetFpsInput_ = 0.0f;

        /**
         * @brief Determines whether timing information is displayed in seconds or milliseconds.
         *
         * If set to `true`, all timing metrics (e.g., frame time, work time, idle time)
         * are shown in seconds. If set to `false`, they are shown in milliseconds.
         *
         * Used to toggle between different units of measurement for presenting
         * frame timing metrics in the FPS widget UI.
         */
        bool showInSeconds_ = false;

        /**
         * @brief Input value for configuring the size of the FPS history buffer.
         *
         * Determines the number of recent frame timing samples to store and display.
         * This value directly influences the frame history graph rendering in the FPS widget.
         * Typically adjusted by the user through the UI input field.
         *
         * @note Must be within a valid range, with a lower limit of 1 and an upper limit
         * of 1000 to prevent excessive memory usage or invalid states.
         */
        int historySizeInput_ = 60;

    public:

        /**
         * @brief Constructs the FpsWidget.
         *
         * @param fpsMetrics Pointer to the FpsMetrics instance (must remain valid).
         * @param framePacer Optional pointer to FramePacer for runtime FPS configuration.
         */
        explicit FpsWidget(
                    helios::tooling::FpsMetrics* fpsMetrics,
                    helios::engine::FramePacer* framePacer = nullptr
                ) : fpsMetrics_(fpsMetrics), framePacer_(framePacer)
        {
            if (fpsMetrics_) {
                historySizeInput_ = static_cast<int>(fpsMetrics_->getHistorySize());
            }
            if (framePacer_) {
                targetFpsInput_ = framePacer_->getTargetFps();
            }
        }

        /**
         * @brief Renders the FPS metrics UI using ImGui.
         */
        void draw() override {
            if (!fpsMetrics_) {
                return;
            }

            // Window settings
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(320, 250), ImGuiCond_FirstUseEver);

            if (ImGui::Begin("FPS Metrics", nullptr, ImGuiWindowFlags_NoCollapse)) {

                // 1. Main Metrics
                ImGui::SeparatorText("Timing Info");

                float displayFrameTime = showInSeconds_ ? fpsMetrics_->getFrameTimeSeconds() : fpsMetrics_->getFrameTimeMs();
                float displayWorkTime  = showInSeconds_ ? fpsMetrics_->getWorkTimeSeconds() : fpsMetrics_->getWorkTimeMs();
                float displayIdleTime  = showInSeconds_ ? fpsMetrics_->getIdleTimeSeconds() : fpsMetrics_->getIdleTimeMs();
                const char* unit = showInSeconds_ ? "s" : "ms";
                const char* format = showInSeconds_ ? "%.5f %s" : "%.2f %s";

                ImGui::Text("FPS:        %.1f", fpsMetrics_->getFps());
                ImGui::Text("Avg Frame:  "); ImGui::SameLine(); ImGui::Text(format, displayFrameTime, unit);
                ImGui::Text("CPU Work:   "); ImGui::SameLine(); ImGui::Text(format, displayWorkTime, unit);
                ImGui::Text("Idle/Wait:  "); ImGui::SameLine(); ImGui::Text(format, displayIdleTime, unit);
                ImGui::Text("Frame #:    %llu", fpsMetrics_->getFrameCount());

                // 2. Settings
                ImGui::SeparatorText("Settings");

                // Checkbox for unit toggle
                if (ImGui::Checkbox("Show in Seconds", &showInSeconds_)) {
                    // UI state changes; display updates next frame
                }

                // Input for history size
                if (ImGui::InputInt("History Size", &historySizeInput_, 1, 10)) {
                    if (historySizeInput_ < 1) historySizeInput_ = 1;
                    if (historySizeInput_ > 1000) historySizeInput_ = 1000;

                    fpsMetrics_->setHistorySize(static_cast<size_t>(historySizeInput_));
                }

                // Frame pacing configuration (if pacer is available)
                if (framePacer_) {
                    ImGui::SeparatorText("Frame Pacing Config");

                    targetFpsInput_ = framePacer_->getTargetFps();

                    // Quick preset buttons
                    if (ImGui::Button("Uncapped (0)")) {
                        framePacer_->setTargetFps(0.0f);
                        targetFpsInput_ = 0.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("30 FPS")) {
                        framePacer_->setTargetFps(30.0f);
                        targetFpsInput_ = 30.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("60 FPS")) {
                        framePacer_->setTargetFps(60.0f);
                        targetFpsInput_ = 60.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("120 FPS")) {
                        framePacer_->setTargetFps(120.0f);
                        targetFpsInput_ = 120.0f;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("144 FPS")) {
                        framePacer_->setTargetFps(144.0f);
                        targetFpsInput_ = 144.0f;
                    }

                    // Slider for precise control
                    float sliderFps = targetFpsInput_;
                    if (ImGui::DragFloat("Target FPS", &sliderFps, 1.0f, 0.0f, 300.0f, "%.0f")) {
                        framePacer_->setTargetFps(sliderFps);
                        targetFpsInput_ = sliderFps;
                    }
                    if (ImGui::IsItemHovered()) {
                        ImGui::SetTooltip("Set to 0.0 for unlimited (V-Sync might still limit)");
                    }
                }

                // Frame time history graph
                const auto& history = fpsMetrics_->getHistory();
                if (!history.empty()) {
                    ImGui::SeparatorText("History");

                    std::vector<float> frameTimes;
                    frameTimes.reserve(history.size());

                    for (const auto& s : history) {
                        frameTimes.push_back(s.totalFrameTime * 1000.0f);
                    }

                    ImGui::PlotLines("Frame Time (ms)",
                                     frameTimes.data(),
                                     static_cast<int>(frameTimes.size()),
                                     0,
                                     nullptr,
                                     0.0f,
                                     FLT_MAX,
                                     ImVec2(0, 80.0f));
                }
            }
            ImGui::End();
        }
    };
}