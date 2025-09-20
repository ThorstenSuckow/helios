module;

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <string>

export module helios.glfw.application:GLFWFactory;

import :GLFWApplication;

import helios.platform.input;
import helios.glfw.input;
import helios.glfw.window;
import helios.rendering.core;
import helios.rendering.opengl;


export namespace helios::glfw::application {


    class GLFWFactory {

    public:


        static std::unique_ptr<core::InputAdapter> makeInputAdapter() {
            return std::make_unique<input::GLFWInput>();
        }

        static GLFWWindowConfig makeWindowCfg(std::string title) {
            auto cfg = GLFWWindowConfig{};
            cfg.title = std::move(title);

            return cfg;
        }

        static std::unique_ptr<GLFWApplication> makeApplication(
            rendering::core::RenderingDevice* renderingDevice
        ) {
            auto inputManager = std::make_unique<InputManager>(makeInputAdapter());
            auto app = std::make_unique<GLFWApplication>(
                renderingDevice, std::move(inputManager)
            );

            return app;
        }

    };


}