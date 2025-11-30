/**
 * @file ImGuiBackend.ixx
 * @brief Abstract backend interface for ImGui integration with rendering/windowing systems.
 */
module;

#include "imgui.h"


export module helios.ext.imgui.ImGuiBackend;


export namespace helios::ext::imgui {

    /**
     * @class ImGuiBackend
     * @brief Platform-agnostic interface for ImGui backend implementations.
     *
     * Concrete backends (e.g., GLFW+OpenGL, SDL+Vulkan) implement this interface
     * to provide platform-specific initialization, frame setup, and rendering.
     */
    class ImGuiBackend {

    public:

        ImGuiBackend() = default;

        virtual ~ImGuiBackend() = default;

        /**
         * @brief Prepares ImGui for a new frame.
         *
         * Must be called at the beginning of each frame before any ImGui calls.
         * Typically invokes platform-specific `NewFrame()` implementations.
         */
        virtual void newFrame() = 0;

        /**
         * @brief Renders ImGui draw data to the screen.
         *
         * Called after `ImGui::Render()` to submit draw commands to the GPU.
         *
         * @param drawData Pointer to ImGui draw data (typically from `ImGui::GetDrawData()`).
         */
        virtual void renderDrawData(ImDrawData* drawData) = 0;

    };
}
