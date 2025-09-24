module;


export module helios.platform.window.event.core:WindowEvent;

import helios.util;
import helios.event.core;

export namespace helios::platform::window::event::core {

    class  WindowEvent : public helios::event::core::Event {

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