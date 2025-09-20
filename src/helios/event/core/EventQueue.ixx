module;

#include <deque>
#include <memory>

export module helios.event.core:EventQueue;

import :Event;

export namespace helios::event::core {

    class EventQueue {

    private:
        std::deque<std::unique_ptr<Event>> events;

    public:
        EventQueue& push(std::unique_ptr<Event> event) {
            events.push_back(std::move(event));
            return *this;
        }

        bool empty() const noexcept {
            return events.empty();
        }

        std::unique_ptr<Event> popFront() {
            if (events.empty()) {
                return nullptr;
            }

            auto event = std::move(events.front());
            events.pop_front();

            return event;
        }

    };

}