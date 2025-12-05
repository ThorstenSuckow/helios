module;

#include <format>

module helios.app.controller.BasicWindowRenderingController;

import helios.ext.opengl.rendering.OpenGLDevice;
import helios.event.Dispatcher;
import helios.app.controller.Controller;
import helios.window.event.FrameBufferResizeEvent;

namespace helios::app::controller {


    BasicWindowRenderingController::BasicWindowRenderingController(helios::window::Window& window):
    window_(window)
    {}


    void BasicWindowRenderingController::onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) {
        logger_.debug(std::format("onFrameBufferResize ({0}, {1})", e.width, e.height));
        window_.renderTarget().setSize(e.width, e.height);
    }

    void BasicWindowRenderingController::subscribeTo(helios::event::Dispatcher& dispatcher) {
        logger_.debug("Subscribing to dispatcher.");
        dispatcher.subscribe<helios::window::event::FrameBufferResizeEvent>(
            [this](const helios::window::event::FrameBufferResizeEvent& e) {
                onFrameBufferResize(e);
            }
        );
    };

    bool BasicWindowRenderingController::init() {
        return true;
    }

    std::string BasicWindowRenderingController::toString() const noexcept {
        return "BasiCWindowController";
    }

};

