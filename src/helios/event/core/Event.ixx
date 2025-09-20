module;

export module helios.event.core:Event;

export namespace helios::event::core {

    struct Event {
        virtual ~Event() =default;
    };

}