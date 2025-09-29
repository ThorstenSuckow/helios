module;

export module helios.platform.application.controller:BasicWindowRenderingController;

import helios.platform.application.controller.core;

import helios.rendering.core.RenderingDevice;
import helios.event.core;
import helios.platform.window;


using namespace helios::platform::application::controller::core;
using namespace helios::platform::window::event;
using namespace helios::rendering::core;
using namespace helios::event::core;

export namespace helios::platform::application::controller {

    class BasicWindowRenderingController final : public WindowRenderingController {

    private:
        RenderingDevice* renderingDevice_ = nullptr;

    public:
        explicit BasicWindowRenderingController(RenderingDevice* renderingDevice):
        renderingDevice_(renderingDevice)
        {}

        void onFrameBufferResize(const FrameBufferResizeEvent& e) override;

        Controller& subscribeTo(Dispatcher& dispatcher) override;

    };


}