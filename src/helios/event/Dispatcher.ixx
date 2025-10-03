module;

#include <functional>
#include <memory>
#include <typeindex>

export module helios.event.Dispatcher;

import helios.event.Event;


export namespace helios::event {

    class Dispatcher {

    private:
        std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> callbacks_;

    public:

        template<typename EventType>
        void subscribe(std::function<void(const EventType&)> callback) {
            static_assert(std::is_base_of_v<Event, EventType>, "EventType is not of type Event");

            const auto idx = std::type_index(typeid(EventType));

            auto wrapper = [callback](const Event& event) {
                const auto& typedEvent = static_cast<const EventType&>(event);
                callback(typedEvent);
            };

            callbacks_[idx].push_back(wrapper);
        }


        void dispatch(std::unique_ptr<const Event> event) {
            const auto idx = std::type_index(typeid(*event));

            if (const auto cb = callbacks_.find(idx); cb != callbacks_.end()) {
                for (const auto& callback : cb->second) {
                    callback(*event);
                }
            }
        }
    };

}