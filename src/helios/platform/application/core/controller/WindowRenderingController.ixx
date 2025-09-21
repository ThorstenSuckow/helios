module;

export module helios.platform.application.core.controller:WindowRenderingController;

import helios.platform.application.core;
import helios.rendering.core;
import helios.event.core;
import helios.platform.window.core;


using namespace helios::platform::application::core;
using namespace helios::platform::window::core::event;
using namespace helios::rendering::core;
using namespace helios::event::core;

export namespace helios::platform::application::core::controller {

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