module;

#include <memory>

module helios.app.Application;

import helios.app.controller.Controller;
import helios.rendering.RenderingDevice;
import helios.input.InputManager;
import helios.event.EventManager;

namespace helios::app {

     Application::Application(
               std::unique_ptr<helios::rendering::RenderingDevice> renderingDevice,
               std::unique_ptr<helios::input::InputManager> inputManager,
               std::unique_ptr<helios::event::EventManager> eventManager
               ):
               renderingDevice_(std::move(renderingDevice)),
               inputManager_(std::move(inputManager)),
               eventManager_(std::move(eventManager))
    {
    };

    Application& Application::addController(std::unique_ptr<helios::app::controller::Controller> controller) {
        controller->subscribeTo(eventManager_->dispatcher());
        controller_.push_back(std::move(controller));
        return *this;
    }
}
