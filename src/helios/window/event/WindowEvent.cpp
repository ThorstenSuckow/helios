module;

#include <cstdint>

module helios.window.event.WindowEvent;

import helios.util.Guid;
import helios.event.Event;

namespace helios::window::event {

    WindowEvent::WindowEvent(helios::util::Guid sourceGuid):
    sourceGuid(sourceGuid)  {}

    WindowEvent::WindowEvent(helios::util::Guid sourceGuid, uint64_t tag):
    Event(tag), sourceGuid(sourceGuid)  {}

}