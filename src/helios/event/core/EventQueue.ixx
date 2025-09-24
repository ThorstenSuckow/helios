module;

#include <deque>
#include <functional>
#include <memory>

export module helios.event.core:EventQueue;

import :Event;

export namespace helios::event::core {

    class EventQueue {

    private:
        std::deque<std::unique_ptr<const Event>> events;

    public:
        EventQueue& push(std::unique_ptr<const Event> event) {
            events.push_back(std::move(event));
            return *this;
        }

        EventQueue& pushOrReplace(
            std::unique_ptr<const Event> event,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& cmpFunc) {

            std::erase_if(events, [&event, cmpFunc](const std::unique_ptr<const Event>& e) {
                return cmpFunc(event, e);
            });
            events.push_back(std::move(event));
            return *this;
        }

        [[nodiscard]] bool empty() const noexcept {
            return events.empty();
        }

        std::unique_ptr<const Event> popFront() {
            if (events.empty()) {
                return nullptr;
            }

            auto event = std::move(events.front());
            events.pop_front();

            return event;
        }

    };

}