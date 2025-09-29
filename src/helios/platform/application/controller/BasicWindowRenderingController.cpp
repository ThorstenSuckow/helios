module;

#include <iostream>

module helios.platform.application.controller;

import helios.platform.application.controller.core;

import helios.platform.application.core;
import helios.rendering.core.RenderingDevice;
import helios.event.core;
import helios.platform.window;


using namespace helios::platform::application::controller::core;
using namespace helios::platform::application;
using namespace helios::platform::application::core;
using namespace helios::platform::window::event;
using namespace helios::rendering::core;
using namespace helios::event::core;

namespace helios::platform::application::controller {

    void BasicWindowRenderingController::onFrameBufferResize(const FrameBufferResizeEvent& e) {
        std::cout << e.width << " " << e.height << std::endl;
    }

    Controller& BasicWindowRenderingController::subscribeTo(Dispatcher& dispatcher) {

        dispatcher.subscribe<FrameBufferResizeEvent>(
            [this](const FrameBufferResizeEvent& e) {
                onFrameBufferResize(e);
            }
        );

        return *this;

    };

};

