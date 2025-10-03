module;

export module helios.app.controller.WindowRenderingController;

import helios.app.controller.Controller;
import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;

export namespace helios::app::controller {

    /**
     * Default controller for coordinating rendering device operations
     * in response to/ and native window events.
     *
     */
    class WindowRenderingController : public Controller {

    public:
        virtual void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) = 0;
    };


}