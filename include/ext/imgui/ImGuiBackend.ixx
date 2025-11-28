
module;

#include <stdexcept>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

export module helios.ext.imgui.ImGuiBackend;


export namespace helios::ext::imgui {

    class ImGuiBackend {
    private:



    public:

        ImGuiBackend() = default;

        virtual ~ImGuiBackend() = default;

        virtual void newFrame() = 0;

        virtual void renderDrawData(ImDrawData* drawData) = 0;

        virtual void shutdown() = 0;
    };
}
