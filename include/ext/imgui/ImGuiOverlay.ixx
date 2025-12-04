/**
 * @file ImGuiOverlay.ixx
 * @brief Manages a collection of ImGui widgets and renders them using a backend.
 */
module;

#include <vector>

#include "imgui.h"

export module helios.ext.imgui.ImGuiOverlay;

import helios.ext.imgui.ImGuiBackend;
import helios.ext.imgui.ImGuiWidget;


export namespace helios::ext::imgui {

    /**
     * @class ImGuiOverlay
     * @brief Central manager for ImGui widgets rendered via a specific backend.
     *
     * The overlay is a singleton per backend. Widgets can be registered dynamically,
     * and all widgets are rendered in the order they were added.
     *
     * @note This class is implemented as a singleton using `forBackend()`. Direct
     * construction is not allowed.
     */
    class ImGuiOverlay {


    private:
        /**
         * @brief Container for dynamically registered ImGui widgets.
         *
         * Stores pointers to widgets that are managed and rendered by the `ImGuiOverlay`.
         * Widgets are added in the order they are registered and must remain valid for the
         * lifetime of the overlay. The container does not take ownership of the widgets.
         *
         * @note This is a private member of `ImGuiOverlay` and is utilized for managing the
         * rendering lifecycle of widgets.
         */
        std::vector<helios::ext::imgui::ImGuiWidget*> widgets_;

        /**
         * @brief Pointer to the backend used for rendering ImGui widgets.
         *
         * Represents the platform-specific implementation of ImGui functionality
         * (e.g., OpenGL, Vulkan, etc.). The backend facilitates frame management,
         * new frame initialization, and rendering draw data.
         *
         * @note This pointer is initialized via the `ImGuiOverlay` constructor
         * and must remain a valid instance for the lifetime of the `ImGuiOverlay`.
         */
        ImGuiBackend* backend_;

        /**
         * @brief Whether to enable a full-viewport DockSpace for docking widgets.
         */
        bool enableDockSpace_ = true;

        /**
         * @brief Constructs an ImGuiOverlay instance with a specified backend.
         *
         * This constructor initializes the overlay with the provided ImGui backend.
         * It is private to enforce the singleton pattern of the overlay.
         *
         * @param backend Pointer to the ImGuiBackend implementation. Must remain valid
         * for the lifetime of the overlay.
         */
        explicit ImGuiOverlay(ImGuiBackend* backend) :
        backend_(backend)
        {
        }

    public:

        /**
         * @brief Retrieves the singleton overlay instance for a given backend.
         *
         * @param backend Pointer to the ImGui backend (e.g., GLFW+OpenGL). Must remain valid.
         * @return Reference to the singleton overlay instance.
         */
        static ImGuiOverlay& forBackend(ImGuiBackend* backend) {

            static auto overlay = ImGuiOverlay(backend);

            return overlay;
        }

        /**
         * @brief Adds a widget to the overlay.
         *
         * Widgets are rendered in the order they were added. The overlay does not
         * take ownership; the caller must ensure the widget remains valid.
         *
         * @param widget Pointer to the widget. Must remain valid for the lifetime of the overlay.
         */
        void addWidget(ImGuiWidget* widget) {

            widgets_.emplace_back(widget);
        }

        /**
         * @brief Enables or disables the full-viewport DockSpace.
         *
         * When enabled, widgets can be docked to the edges of the main viewport.
         *
         * @param enabled True to enable docking, false to disable.
         */
        void setDockSpaceEnabled(bool enabled) noexcept {
            enableDockSpace_ = enabled;
        }

        /**
         * @brief Renders all registered widgets using the backend.
         *
         * Calls `newFrame()` on the backend, optionally creates a DockSpace,
         * invokes `draw()` on each widget, then finalizes rendering via
         * `ImGui::Render()` and `renderDrawData()`.
         */
        void render() {
            backend_->newFrame();

            // Create a full-viewport DockSpace so widgets can be docked to window edges
            if (enableDockSpace_) {
                ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(),
                                              ImGuiDockNodeFlags_PassthruCentralNode);
            }

            for (const auto it : widgets_) {
                it->draw();
            }

            ImGui::Render();
            backend_->renderDrawData(ImGui::GetDrawData());
        }

    };


}