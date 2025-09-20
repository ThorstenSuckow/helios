module;


export module helios.platform.window.core.event:FrameBufferResizeEvent;

import :WindowEvent;

import helios.util.Guid;

export namespace helios::platform::window::core::event {

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