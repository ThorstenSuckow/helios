module;

export module helios.platform.application.controller.core:WindowRenderingController;

import :Controller;
import helios.rendering.core;
import helios.event.core;
import helios.platform.window.core;


using namespace helios::platform::application::controller;
using namespace helios::platform::window::core::event;
using namespace helios::rendering::core;
using namespace helios::event::core;

export namespace helios::platform::application::controller::core {

    /**
     * Default controller for coordinating rendering device operations
     * in response to/ and native window events.
     *
     */
    class WindowRenderingController : public Controller {

    public:
        virtual void onFrameBufferResize(const FrameBufferResizeEvent& e) = 0;
    };


}