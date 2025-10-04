module;

#include <cstdint>

module helios.window.event.FrameBufferResizeEvent;

import helios.window.event.WindowEvent;

import helios.util.Guid;

using namespace helios::window::event;

namespace helios::window::event {

    FrameBufferResizeEvent::FrameBufferResizeEvent(
        util::Guid sourceGuid, const int width, const int height
    ) noexcept :
    WindowEvent(sourceGuid),
    width(width),
    height(height)
            {}


    FrameBufferResizeEvent::FrameBufferResizeEvent(
        util::Guid sourceGuid, const int width, const int height, const uint64_t tag
    ) noexcept :
    WindowEvent(sourceGuid, tag),
    width(width),
    height(height)
    {}

};

