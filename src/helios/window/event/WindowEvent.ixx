module;

#include <cstdint>

export module helios.window.event.WindowEvent;

import helios.util.Guid;
import helios.event.Event;

export namespace helios::window::event {

    class  WindowEvent : public helios::event::Event {

    protected:
        explicit WindowEvent(helios::util::Guid sourceGuid):
        sourceGuid(sourceGuid)  {}

        explicit WindowEvent(helios::util::Guid sourceGuid, uint64_t tag):
        Event(tag), sourceGuid(sourceGuid)  {}

    public:
        helios::util::Guid sourceGuid;

        virtual ~WindowEvent() = default;
    };

}