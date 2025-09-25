module;

#include <deque>
#include <functional>
#include <memory>

export module helios.event:DequeEventQueue;

import helios.event.core;

using namespace helios::event::core;

export namespace helios::event {

    class DequeEventQueue : public EventQueue{

    private:
        std::deque<std::unique_ptr<const Event>> events;

    public:
        EventQueue& add(std::unique_ptr<const Event> event) override;

        EventQueue& addOrReplace(
            std::unique_ptr<const Event> event,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& cmpFunc) override;

        [[nodiscard]] bool empty() const noexcept override;

        std::unique_ptr<const Event> next() override;

    };

}