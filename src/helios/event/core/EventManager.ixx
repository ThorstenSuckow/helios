module;

#include <memory>

export module helios.event.core:EventManager;

import :EventQueue;
import :Dispatcher;
import :Event;

export namespace helios::event::core {

    class EventManager {

    private:
        std::unique_ptr<EventQueue> eventQueue_;
        std::unique_ptr<Dispatcher> dispatcher_;

    public:
        EventManager(std::unique_ptr<EventQueue> eventQueue,
            std::unique_ptr<Dispatcher> dispatcher) :
            eventQueue_(std::move(eventQueue)),
            dispatcher_(std::move(dispatcher))
        {}

        EventManager& post(std::unique_ptr<Event> e) {
            eventQueue_->push(std::move(e));
            return *this;
        }

        EventManager& dispatchAll() {
            while (!eventQueue_->empty()) {
                auto e = eventQueue_->popFront();
                dispatcher_->dispatch(std::move(e));
            }

            return *this;
        }

        template<typename EventType>
        EventManager& subscribe(std::function<void(const EventType&)> callback) {
            dispatcher_->subscribe(callback);

            return *this;
        }




    };

}