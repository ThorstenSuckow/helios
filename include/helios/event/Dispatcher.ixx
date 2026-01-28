/**
 * @file Dispatcher.ixx
 * @brief Dispatches events to registered callbacks.
 */
module;

#include <functional>
#include <memory>
#include <typeindex>

export module helios.event.Dispatcher;

import helios.event.Event;


export namespace helios::event {

    /**
     * @brief A generic event Dispatcher for type-safe event handling.
     *
     * The `Dispatcher` allows interested entities to subscribe to specific
     * event types and dispatch events in a decoupled manner.
     * Using `std::type_index` makes sure that only relevant handlers receive
     * specific event instances.
     *
     */
    class Dispatcher {

    private:
        /**
         * @brief Internal map to store registered callbacks for various event types.
         */
        std::unordered_map<std::type_index, std::vector<std::function<void(const Event&)>>> callbacks_;

    public:
        /**
         * @brief Subscribes a callback to a specific event type.
         *
         * Registers a `std::function`  to be invoked when an event of `EventType`
         * is dispatched. The callback will receive the dispatched event as a const reference.
         *
         * @tparam EventType The specific type of event to subscribe to. Must derive from `Event`.
         * @param callback The callback function to be executed when the event is dispatched. The
         * function should accept `const EventType&` as its single argument.
         */
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


        /**
         * @brief Dispatches an event to all subscribed callbacks (i.e. listeners) of the
         * specific EventType.
         *
         * This method will take ownership of the event to dispatch.
         * Once this method returns, the unique_ptr to the `Event` will be a nullptr.
         *
         * @param event A unique_ptr to the event instance to be dispatched.
         */
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