/**
 * @file ImGuiWidget.ixx
 * @brief Base interface for ImGui-based debug and UI widgets.
 */
module;


export module helios.ext.imgui.ImGuiWidget;


export namespace helios::ext::imgui {

    /**
     * @class ImGuiWidget
     * @brief Abstract base class for ImGui widgets rendered in debug overlays.
     *
     * All widgets must implement the `draw()` method, which is called once per
     * frame by the owning `ImGuiOverlay`. Widgets are typically used for real-time
     * debugging, profiling, and developer tools.
     *
     * @note Widgets are not thread-safe and must be used from the main/render thread.
     */
    class ImGuiWidget {

    public:
        ImGuiWidget() = default;
        virtual ~ImGuiWidget() = default;

        /**
         * @brief Renders the widget using ImGui immediate-mode API.
         *
         * Called once per frame by the owning overlay. Implementers should call
         * ImGui functions (e.g., `ImGui::Begin()`, `ImGui::Text()`) to build the UI.
         */
        virtual void draw() = 0;

    };


}