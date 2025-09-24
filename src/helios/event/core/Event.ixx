module;

#include <memory>

export module helios.event.core:Event;

import helios.util;


using namespace helios::util;

export namespace helios::event::core {

    /**
     * @todo can we reset the value for Guids in between frames, if events
     * are guaranteed to be dispatched and processed in between frames?
     */
    class Event {
    private:
        const Guid guid_;
        const uint64_t tag_;

    public:
        Event(): guid_(Guid::generate()), tag_(0) {};
        explicit Event(const uint64_t tag): guid_(Guid::generate()), tag_(tag) {};

        virtual ~Event() =default;

        [[nodiscard]] const Guid& guid() const {
            return guid_;
        }

        bool operator==(const Event& e) const {
            return guid_ == e.guid();
        }

        [[nodiscard]] uint64_t tag() const {
            return tag_;
        }


    };

}