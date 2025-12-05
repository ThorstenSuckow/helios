module;

#include <memory>
#include <stdexcept>
#include <iostream>

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


    [[nodiscard]] bool Application::hasWindow(const helios::window::Window& win) const noexcept {
        auto it = std::find_if(
       windowList_.begin(),
       windowList_.end(),
       [&win](const auto& window){return *window == win;});

        return it != windowList_.end();
    }


    void Application::init () {

        logger_.info("Initializing application.");

        if (initialized_) {
            std::string msg = "Application was already initialized.";
            logger_.error(msg);
            throw std::runtime_error(msg);
        }
        for (auto& ctrl: controllers_) {
            if (ctrl->init()) {
                ctrl->subscribeTo(eventManager_->dispatcher());
            }
        }

        initialized_ = true;
    }

    void Application::addController(std::unique_ptr<helios::app::controller::Controller> controller) noexcept {
        if (initialized_) {
            logger_.info(
                std::format("Controller {0} added to an already initialized Application, explicitly initializing.", controller->toString())
                );
            if (controller->init()) {
                controller->subscribeTo(eventManager_->dispatcher());
            }
        }
        controllers_.push_back(std::move(controller));
    }
}
