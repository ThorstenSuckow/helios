module;

#include <deque>
#include <functional>
#include <memory>

module helios.event;

import helios.event.core;

using namespace helios::event::core;

namespace helios::event {


    EventQueue& DequeEventQueue::add(std::unique_ptr<const Event> event) {
        events.push_back(std::move(event));
        return *this;
    }

    EventQueue& DequeEventQueue::addOrReplace(
        std::unique_ptr<const Event> event,
        const std::function<bool(const std::unique_ptr<const Event>& evt,
            const std::unique_ptr<const Event>& e)>& cmpFunc) {

        std::erase_if(events, [&event, cmpFunc](const std::unique_ptr<const Event>& e) {
            return cmpFunc(event, e);
        });
        events.push_back(std::move(event));
        return *this;
    }

    [[nodiscard]] bool DequeEventQueue::empty() const noexcept {
        return events.empty();
    }

    std::unique_ptr<const Event> DequeEventQueue::next() {
        if (events.empty()) {
            return nullptr;
        }

        auto event = std::move(events.front());
        events.pop_front();

        return event;
    }

};

