/**
 * @file ImGuiGlfwOpenGLBackend.ixx
 * @brief GLFW+OpenGL backend implementation for ImGui rendering.
 */
module;

#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

export module helios.ext.imgui.ImGuiGlfwOpenGLBackend;

import helios.ext.imgui.ImGuiBackend;

export namespace helios::ext::imgui {

    /**
     * @class ImGuiGlfwOpenGLBackend
     * @brief ImGui backend for GLFW windowing and OpenGL 4.6 rendering.
     *
     * Initializes ImGui context, GLFW platform layer, and OpenGL renderer.
     * This backend is non-copyable and non-movable due to resource ownership semantics.
     *
     * @note Only one instance should exist per application. Creating multiple instances
     * will throw a `std::runtime_error`.
     */
    class ImGuiGlfwOpenGLBackend : public ImGuiBackend {

    private:

        /**
         * @brief Indicates whether the backend has been successfully initialized.
         *
         * Tracks the initialization state of the ImGui backend to prevent multiple
         * redundant initialization attempts and manage proper shutdown procedures.
         *
         * @note Modified during initialization and shutdown processes.
         */
        bool initialized_ = false;

        /**
         * @brief Shuts down the ImGui GLFW and OpenGL backend.
         *
         * Ensures proper cleanup of the ImGui context, OpenGL, and GLFW resources.
         * After calling this method, the backend will no longer be initialized, and
         * associated resources will be released.
         *
         * This function is safe to call multiple times but has no effect if the backend
         * is not currently initialized.
         *
         * @note This method is noexcept and will not throw exceptions.
         */
        void shutdown() noexcept {
            if (initialized_) {
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
                initialized_ = false;
            }
        }

    public:

        // No copy, no move (manages global ImGui context).
        ImGuiGlfwOpenGLBackend(const ImGuiGlfwOpenGLBackend&) = delete;
        ImGuiGlfwOpenGLBackend& operator=(const ImGuiGlfwOpenGLBackend&) = delete;
        ImGuiGlfwOpenGLBackend(ImGuiGlfwOpenGLBackend&& other) noexcept = delete;
        ImGuiGlfwOpenGLBackend& operator=(ImGuiGlfwOpenGLBackend&& other) noexcept = delete;

        /**
         * @brief Constructs and initializes the ImGui backend for GLFW+OpenGL.
         *
         * @param window GLFW window handle. Must be valid for the lifetime of this backend.
         *
         * @throws std::runtime_error if ImGui context already exists or initialization fails.
         */
        explicit ImGuiGlfwOpenGLBackend(GLFWwindow* window) {

            if (ImGui::GetCurrentContext()) {
                throw std::runtime_error("ImGui context already initialized");
            }

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            ImGui::StyleColorsDark();

            if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
                ImGui::DestroyContext();
                throw std::runtime_error("Failed to initialize GLFW backend for ImGui");
            }

            if (!ImGui_ImplOpenGL3_Init("#version 460")) {
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();
                throw std::runtime_error("Failed to initialize OpenGL 4.6 backend for ImGui");
            }
            initialized_ = true;
        }

        /**
         * @brief Renders ImGui draw data using OpenGL.
         *
         * @param drawData Pointer to ImGui draw data.
         */
        void renderDrawData(ImDrawData* drawData) override {
            ImGui_ImplOpenGL3_RenderDrawData(drawData);
        }

        /**
         * @brief Starts a new ImGui frame.
         */
        void newFrame() override {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        /**
         * @brief Destructor; shuts down ImGui backend and releases resources.
         *
         * @see shutdown()
         */
        ~ImGuiGlfwOpenGLBackend() override {
            shutdown();
        }

    };

}
