module;

#include <functional>
#include <memory>
#include <utility>

export module helios.event.EventManager;

import helios.event.Event;
import helios.event.EventQueue;
import helios.event.Dispatcher;

export namespace helios::event {


    enum PostPolicy {
        LATEST_WINS,
        APPEND
    };


    class EventManager {

    protected:
        std::unique_ptr<EventQueue> eventQueue_;
        std::unique_ptr<Dispatcher> dispatcher_;

        EventManager() = default;

    public:
        EventManager(std::unique_ptr<EventQueue> eventQueue,
            std::unique_ptr<Dispatcher> dispatcher) :
            eventQueue_(std::move(eventQueue)),
            dispatcher_(std::move(dispatcher))
        {}

        virtual ~EventManager() = default;

        /**
         * Posts the event based on the specified Policy.
         * func serves as a comparision function, e.g. if LATEST_WINS
         * is used.
         *
         * @param e
         * @param policy
         * @param func
         * @return
         */
        virtual EventManager& post(
            std::unique_ptr<const Event> e,
            PostPolicy policy,
            const std::function<bool(
                const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& func
        ) = 0;

        EventManager& post(std::unique_ptr<const Event> e) {
            return post(std::move(e), APPEND, nullptr);
        };

        EventManager& post(std::unique_ptr<const Event> e, PostPolicy policy) {
            return post(std::move(e), policy, nullptr);
        };

        /**
         * Dispatches all events and flushes the queue.
         *
         * @return
         */
        virtual EventManager& dispatchAll() = 0;

        template<typename EventType>
        EventManager& subscribe(std::function<void(const EventType&)> callback) {
            dispatcher_->subscribe(callback);

            return *this;
        }


        [[nodiscard]] Dispatcher& dispatcher() const noexcept {
            return *dispatcher_;
        }

    };

}