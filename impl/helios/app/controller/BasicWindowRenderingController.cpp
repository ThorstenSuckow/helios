module;

#include <iostream>

module helios.app.controller.BasicWindowRenderingController;

import helios.event.Dispatcher;
import helios.app.controller.Controller;
import helios.window.event.FrameBufferResizeEvent;

namespace helios::app::controller {


    BasicWindowRenderingController::BasicWindowRenderingController(helios::rendering::RenderingDevice* renderingDevice):
    renderingDevice_(renderingDevice)
    {}


    void BasicWindowRenderingController::onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) {
        std::cout << e.width << " " << e.height << std::endl;
    }


    void BasicWindowRenderingController::subscribeTo(helios::event::Dispatcher& dispatcher) {

        dispatcher.subscribe<helios::window::event::FrameBufferResizeEvent>(
            [this](const helios::window::event::FrameBufferResizeEvent& e) {
                onFrameBufferResize(e);
            }
        );
    };

};

