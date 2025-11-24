/**
 * @file BasicWindowRenderingController.ixx
 * @brief Controller implementation for basic window rendering management.
 */
module;

export module helios.app.controller.BasicWindowRenderingController;

import helios.app.controller.WindowRenderingController;

import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;
import helios.event.Dispatcher;
import helios.window.Window;

export namespace helios::app::controller {

    /**
     * @brief Basic implementation of a window rendering controller.
     *
     * This controller handles framebuffer resize events and manages rendering
     * for a single window instance.
     */
    class BasicWindowRenderingController final : public WindowRenderingController {

    private:
        /**
         * @brief Reference to the window managed by this controller.
         */
        helios::window::Window& window_;

    public:
        /**
         * @brief Constructs a new BasicWindowRenderingController instance.
         *
         * @param window Reference to the window to be managed by this controller.
         */
        explicit BasicWindowRenderingController(helios::window::Window& window);

        /**
         * @brief Handles framebuffer resize events.
         *
         * @param e The framebuffer resize event containing the new dimensions.
         */
        void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) override;

        /**
         * @brief Registers this controller to the framebuffer resize event.
         *
         * @param dispatcher The event dispatcher to subscribe to.
         *
         * @see onFrameBufferResize
         */
        void subscribeTo(helios::event::Dispatcher& dispatcher) override;

        /**
         * @brief Initializes this controller.
         *
         * @return True if initialization succeeded, otherwise false.
         */
        bool init() override;
    };


}