/**
 * @file WindowRenderingController.ixx
 * @brief Controller coordinating rendering for a window.
 */
module;

export module helios.app.controller.WindowRenderingController;

import helios.app.controller.Controller;
import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;

export namespace helios::app::controller {

    /**
     * @brief Abstract base controller for handling window related events.
     *
     * Concrete implementations will typically update the rendering device's viewport when the framebuffer
     * dimension changes.
     */
    class WindowRenderingController : public Controller {

    public:
        /**
         * @brief Handles FrameBuffer resize events.
         *
         * @param e A const reference to the FrameBufferResizeEvent describing the event.
         */
        virtual void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) = 0;
    };


}