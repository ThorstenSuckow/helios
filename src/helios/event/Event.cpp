module;

#include <memory>

module helios.event.Event;

import helios.util.Guid;

namespace helios::event {

    Event::Event(): guid_(helios::util::Guid::generate()), tag_(0) {};


    Event::Event(const uint64_t tag): guid_(helios::util::Guid::generate()), tag_(tag) {};


    [[nodiscard]] const helios::util::Guid& Event::guid() const {
        return guid_;
    }


    [[nodiscard]] uint64_t Event::tag() const {
        return tag_;
    }


    bool Event::operator==(const Event& e) const {
        return guid_ == e.guid();
    }

};
