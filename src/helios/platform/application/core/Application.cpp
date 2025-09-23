module;

#include <memory>

module helios.platform.application.core;

import helios.platform.application.controller.core;

import helios.rendering.core;
import helios.platform.input;
import helios.platform.window.core;
import helios.event.core;


using namespace helios::platform::window::core;
using namespace helios::platform::application::controller::core;

namespace helios::platform::application::core {

    Application& Application::addController(std::unique_ptr<Controller> controller) {
        controller->subscribeTo(eventManager_->dispatcher());
        controller_.push_back(std::move(controller));
        return *this;
    }
}
