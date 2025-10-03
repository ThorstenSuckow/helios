module;

#include <functional>
#include <memory>
#include <utility>

module helios.event.EventManager;

import helios.event.Event;
import helios.event.EventQueue;
import helios.event.Dispatcher;

namespace helios::event {


    EventManager::EventManager(std::unique_ptr<EventQueue> eventQueue,
        std::unique_ptr<Dispatcher> dispatcher) :
        eventQueue_(std::move(eventQueue)),
        dispatcher_(std::move(dispatcher))
    {}


    EventManager& EventManager::post(std::unique_ptr<const Event> event) {
        return post(std::move(event), APPEND, nullptr);
    };


    EventManager& EventManager::post(std::unique_ptr<const Event> event, PostPolicy policy) {
        return post(std::move(event), policy, nullptr);
    };


    template<typename EventType>
    EventManager& EventManager::subscribe(std::function<void(const EventType&)> callback) {
        dispatcher_->subscribe(callback);

        return *this;
    }


    [[nodiscard]] Dispatcher& EventManager::dispatcher() const noexcept {
        return *dispatcher_;
    }

};

