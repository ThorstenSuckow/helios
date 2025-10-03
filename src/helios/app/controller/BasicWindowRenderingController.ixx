module;

export module helios.app.controller.BasicWindowRenderingController;

import helios.app.controller.WindowRenderingController;

import helios.rendering.RenderingDevice;
import helios.window.event.FrameBufferResizeEvent;
import helios.event.Dispatcher;

export namespace helios::app::controller {

    class BasicWindowRenderingController final : public WindowRenderingController {

    private:
        helios::rendering::RenderingDevice* renderingDevice_ = nullptr;

    public:
        explicit BasicWindowRenderingController(helios::rendering::RenderingDevice* renderingDevice):
        renderingDevice_(renderingDevice)
        {}

        void onFrameBufferResize(const helios::window::event::FrameBufferResizeEvent& e) override;

        Controller& subscribeTo(helios::event::Dispatcher& dispatcher) override;

    };


}