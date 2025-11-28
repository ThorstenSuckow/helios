
module;

#include <stdexcept>


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

export module helios.ext.imgui.ImGuiGlfwOpenGLBackend;

import helios.ext.imgui.ImGuiBackend;

export namespace helios::ext::imgui {

    class ImGuiGlfwOpenGLBackend : public ImGuiBackend {


    private:
        explicit ImGuiGlfwOpenGLBackend (GLFWwindow* window) {

            /**
             * @todo may only be initialized once for application
             */
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO();
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init("#version 460");

            // Setup ImGui style
            ImGui::StyleColorsDark();
        }


    public:


        static ImGuiGlfwOpenGLBackend& forWindow(GLFWwindow* window) {

            static bool initialized = false;

            /**
             * @todo relax error handling, create backend per window and return.
             */
            if (initialized) {
                throw std::runtime_error("Already intialized");
            }
            initialized = true;

            static auto backend = ImGuiGlfwOpenGLBackend(window);

            return backend;
        }

        void renderDrawData(ImDrawData* drawData) override {
            ImGui_ImplOpenGL3_RenderDrawData(drawData);
        }

        void newFrame() override {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void shutdown() override {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

    };


}
