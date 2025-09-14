module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

export module helios.util.glfw:GLFWFactory;


import helios.platform.input;
import helios.platform.input.glfw;
import helios.platform.input.core;
import helios.platform.window.glfw;
import helios.platform.application.glfw;
import helios.rendering.core;
import helios.rendering.opengl;



using namespace helios::platform;

export namespace helios::util::glfw {


    class GLFWFactory {

    public:

        static std::unique_ptr<input::core::InputAdapter> makeInputAdapter() {
            return std::make_unique<input::glfw::GLFWInput>();
        }

        static window::glfw::GLFWWindowConfig makeWindowCfg(std::string title) {
            auto cfg = window::glfw::GLFWWindowConfig{};
            cfg.title = std::move(title);
            cfg.frameBufferSizeCallback = [](GLFWwindow* win, const int width, const int height) {
                glViewport(0, 0, width, height);
            };

            return cfg;
        }

        static std::unique_ptr<application::glfw::GLFWApplication> makeApplication(
            rendering::core::RenderingDevice* renderingDevice
        ) {
            auto inputManager = std::make_unique<input::InputManager>(makeInputAdapter());
            auto app = std::make_unique<application::glfw::GLFWApplication>(
                renderingDevice, std::move(inputManager)
            );

            return app;
        }

    };


}