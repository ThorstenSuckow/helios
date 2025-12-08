module;

#include <cstdint>
#include <format>

module helios.window.event.FrameBufferResizeEvent;

import helios.window.event.WindowEvent;

import helios.util.Guid;

using namespace helios::window::event;

namespace helios::window::event {

    FrameBufferResizeEvent::FrameBufferResizeEvent(
        const util::Guid sourceGuid, const int width, const int height
    ) noexcept :
    WindowEvent(sourceGuid),
    width(width),
    height(height)
            {}


    FrameBufferResizeEvent::FrameBufferResizeEvent(
        const util::Guid sourceGuid, const int width, const int height, const uint64_t tag
    ) noexcept :
    WindowEvent(sourceGuid, tag),
    width(width),
    height(height)
    {}

    std::string FrameBufferResizeEvent::toString() const noexcept {
        return std::format(
            "FrameBufferResizeEvent[{0}, {1}, {2}, {3}]",
            sourceGuid.value(),
            tag(),
            width,
            height
        );

    }

};

