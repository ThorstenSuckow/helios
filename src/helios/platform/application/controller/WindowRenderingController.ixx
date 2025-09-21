module;

#include <iostream>

export module helios.platform.application.controller:WindowRenderingController;


import helios.platform.application.core.controller;

import helios.platform.application.core;
import helios.rendering.core;
import helios.event.core;
import helios.platform.window.core;


using namespace helios::platform::application::core;
using namespace helios::platform::window::core::event;
using namespace helios::rendering::core;
using namespace helios::event::core;

export namespace helios::platform::application::controller {

    class WindowRenderingController : public core::controller::WindowRenderingController {

    private:
        RenderingDevice* renderingDevice_ = nullptr;

    public:
        WindowRenderingController(RenderingDevice* renderingDevice):
            renderingDevice_(renderingDevice)
        {}

        void onFrameBufferResize(const FrameBufferResizeEvent& e) override {
            std::cout << e.width << " " << e.height << std::endl;
        }

        Controller& subscribeTo(Dispatcher& dispatcher) override {

            dispatcher.subscribe<FrameBufferResizeEvent>(
                [this](const FrameBufferResizeEvent& e) {
                    onFrameBufferResize(e);
                }
            );

            return *this;

        };

    };


}