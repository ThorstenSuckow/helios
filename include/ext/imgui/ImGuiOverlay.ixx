module;

#include <vector>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


export module helios.ext.imgui.ImGuiOverlay;

import helios.ext.imgui.ImGuiBackend;
import helios.ext.imgui.ImGuiWidget;


export namespace helios::ext::imgui {

    class ImGuiOverlay {


    private:
        std::vector<helios::ext::imgui::ImGuiWidget*> widgets_;

        ImGuiBackend* backend_;

        explicit ImGuiOverlay(ImGuiBackend* backend) :
        backend_(backend)

        {

        }



    public:



        static ImGuiOverlay& forBackend(ImGuiBackend* backend) {

            static auto overlay = ImGuiOverlay(backend);

            return overlay;
        }

        void addWidget(ImGuiWidget* widget) {

            widgets_.emplace_back(widget);
        }

        void render() {
            backend_->newFrame();

            for (const auto it : widgets_) {
                it->draw();
            }

            ImGui::End();
            ImGui::Render();
            backend_->renderDrawData(ImGui::GetDrawData());
        }



    };


}