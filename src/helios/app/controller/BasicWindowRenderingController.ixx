module;

export module helios.app.controller.BasicWindowRenderingController;

import helios.app.controller.WindowRenderingController;

import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;
import helios.event.Dispatcher;

export namespace helios::app::controller {

    /**
     * Basic implementation of a WindowRenderingController.
     *
     */
    class BasicWindowRenderingController final : public WindowRenderingController {

    private:
        helios::rendering::RenderingDevice* renderingDevice_ = nullptr;

    public:
        explicit BasicWindowRenderingController(helios::rendering::RenderingDevice* renderingDevice);

        /**
         * Handles FrameBufferResizeEvent.
         * The current implementation simply logs the dimensions to the console.
         *
         * @param e
         */
        void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) override;

        /**
         * Registers to the FrameBufferResizeEvent of the specified dispatcher.
         *
         * @param dispatcher
         *
         *  @see onFrameBufferResize
         */
        void subscribeTo(helios::event::Dispatcher& dispatcher) override;

        /**
         * Initializes this BasicWindowRenderingController.
         */
        bool init() override;
    };


}