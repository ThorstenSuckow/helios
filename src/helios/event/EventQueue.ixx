module;

#include <deque>
#include <functional>
#include <memory>

export module helios.event.EventQueue;

import helios.event.Event;

export namespace helios::event {

    class EventQueue {

    public:
        virtual ~EventQueue() = default;

        virtual EventQueue& add(std::unique_ptr<const Event> event) = 0;

        virtual EventQueue& addOrReplace(
            std::unique_ptr<const Event> event,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& cmpFunc) = 0;

        [[nodiscard]] virtual bool empty() const noexcept = 0;

        virtual std::unique_ptr<const Event> next() = 0;

    };

}