module;


export module helios.platform.window.event:FrameBufferResizeEvent;

import helios.platform.window.event.core;

import helios.util.Guid;

using namespace helios::platform::window::event::core;

export namespace helios::platform::window::event {

    struct FrameBufferResizeEvent final : WindowEvent {
        float width;
        float height;

        FrameBufferResizeEvent(
            util::Guid sourceGuid,
            const float width,
            const float height) noexcept :
        WindowEvent(sourceGuid),
        width(width),
        height(height)
        {}

        ~FrameBufferResizeEvent() override = default;
    };

}