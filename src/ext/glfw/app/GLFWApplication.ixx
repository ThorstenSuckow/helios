module;

#include <memory>


export module helios.ext.glfw.app.GLFWApplication;

import helios.app.Application;
import helios.window.Window;
import helios.window.WindowConfig;
import helios.input.InputManager;
import helios.rendering.RenderingDevice;
import helios.event.EventManager;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.window.GLFWWindowConfig;

export namespace helios::ext::glfw::app {

    class GLFWApplication : public helios::app::Application {

    private:
        helios::window::Window* current_ = nullptr;

    public:

        explicit GLFWApplication(
            std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
            std::unique_ptr<helios::input::InputManager> inputManager,
            std::unique_ptr<helios::event::EventManager> eventManager):
            Application(
                std::move(renderingDevice),
                std::move(inputManager),
                std::move(eventManager)
                ) {

        }

        helios::ext::glfw::window::GLFWWindow& createWindow(const helios::ext::glfw::window::GLFWWindowConfig& cfg);

        void setCurrent(helios::window::Window& win) override;

        [[nodiscard]] helios::window::Window* current() const noexcept override;


        helios::ext::glfw::window::GLFWWindow& createWindow(const helios::window::WindowConfig& cfg) override;


        void init() override;

    };


}