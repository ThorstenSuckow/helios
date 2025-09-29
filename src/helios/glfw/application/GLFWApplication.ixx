module;

#include <memory>


export module helios.glfw.application:GLFWApplication;

import helios.platform.application.core;
import helios.rendering.core.RenderingDevice;
import helios.glfw.window;
import helios.platform.window.core;
import helios.platform.input.core;
import helios.platform.input;
import helios.event.core;

using namespace helios::platform::application::core;
using namespace helios::glfw::window;
using namespace helios::platform::window::core;
using namespace helios::platform::input;

export namespace helios::glfw::application {

    class GLFWApplication : public Application {

    private:
        Window* current_ = nullptr;

    public:

        explicit GLFWApplication(
            std::unique_ptr<rendering::core::RenderingDevice> renderingDevice,
            std::unique_ptr<InputManager> inputManager,
            std::unique_ptr<event::core::EventManager> eventManager):
            Application(
                std::move(renderingDevice),
                std::move(inputManager),
                std::move(eventManager)
                ) {

        }

        GLFWWindow& createWindow(const GLFWWindowConfig& cfg);

        GLFWApplication& setCurrent(Window& win) override;

        [[nodiscard]] Window* current() const noexcept override;


        GLFWWindow& createWindow(const WindowConfig& cfg) override;


        GLFWApplication& init() override;

    };


}