module;

#include <cstdint>

export module helios.window.event.FrameBufferResizeEvent;

import helios.window.event.WindowEvent;

import helios.util.Guid;

using namespace helios::window::event;

export namespace helios::window::event {

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