module;

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


export module helios.ext.imgui.widgets.FpsWidget;

import helios.ext.imgui.ImGuiWidget;

export namespace helios::ext::imgui::widgets {


    class FpsWidget : public ImGuiWidget {


    public:


        void draw() override {

            // Create ImGui window
            ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);

            ImGui::Begin("Performance Monitor", nullptr, ImGuiWindowFlags_NoCollapse);

            ImGui::Text("FPS: %.1f", 1);
            ImGui::Text("Frame Time: %.2f ms", 2);
            ImGui::Separator();

            // Additional info
            ImGui::Text("Application: Spaceship Control");
            ImGui::Text("Press ESC to exit");

        }

    };


}