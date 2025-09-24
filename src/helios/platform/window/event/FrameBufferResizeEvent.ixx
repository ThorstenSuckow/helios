module;


export module helios.platform.window.event:FrameBufferResizeEvent;

import helios.platform.window.event.core;

import helios.util;

using namespace helios::platform::window::event::core;

export namespace helios::platform::window::event {

    class FrameBufferResizeEvent final : public WindowEvent {

    public:
        const float width;
        const float height;

        FrameBufferResizeEvent(
            util::Guid sourceGuid,
            const float width,
            const float height
        ) noexcept :
        WindowEvent(sourceGuid),
        width(width),
        height(height)
                {}

        FrameBufferResizeEvent(
            util::Guid sourceGuid,
            const float width,
            const float height,
            const uint64_t tag
        ) noexcept :
        WindowEvent(sourceGuid, tag),
        width(width),
        height(height)
        {}


        ~FrameBufferResizeEvent() override = default;
    };

}