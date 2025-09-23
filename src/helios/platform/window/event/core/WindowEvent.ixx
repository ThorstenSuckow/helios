module;


export module helios.platform.window.event.core:WindowEvent;

import helios.util;
import helios.event.core;

export namespace helios::platform::window::event::core {

    struct WindowEvent : helios::event::core::Event {
        helios::util::Guid sourceGuid;

    protected:
        explicit WindowEvent(helios::util::Guid sourceGuid):
        sourceGuid(sourceGuid)  {}

    public:
        virtual ~WindowEvent() = default;
    };

}