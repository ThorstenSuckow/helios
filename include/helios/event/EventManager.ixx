/**
 * @file EventManager.ixx
 * @brief High-level event manager dispatching events to listeners.
 */
module;

#include <functional>
#include <memory>
#include <utility>
#include <format>

export module helios.event.EventManager;

import helios.event.Event;
import helios.event.EventQueue;
import helios.event.Dispatcher;

import helios.util.log.LogManager;
import helios.util.log.Logger;

#define HELIOS_LOG_SCOPE "helios::event::EventManager"
export namespace helios::event {

    /**
     * @brief Defines the posting policy for events.
     *
     * This enum specifies how events should be handled when posted
     * to the EventQueue.
     */
    enum PostPolicy {
        /**
         * @brief Policy for removing an event in favor of a newer one based
         * on specific criteria.
         */
        LATEST_WINS,

        /**
         * @brief Policy for simply adding an event to the underlying EventQueue.
         */
        APPEND,

        /**
         * @brief Number of entries in this enumeration.
         */
        size_
    };


    /**
     * @brief An abstract EventManager managing the buffering and dispatching of
     * events, acting as a central hub for event management.
     *
     * Concrete implementations take care of `dispatchAll()`, that is, the
     * logic for dispatching all events to the subscribed callbacks.
     */
    class EventManager {

    protected:

        static inline const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(HELIOS_LOG_SCOPE);

        /**
         * @brief A unique_ptr to the EventQueue this EventManager owns.
         */
        std::unique_ptr<EventQueue> eventQueue_;

        /**
         * @brief A unique_ptr to the Dispatcher this EventManager owns.
        */
        std::unique_ptr<Dispatcher> dispatcher_;

        EventManager() = default;

    public:

        virtual ~EventManager() = default;

        /**
         * @brief Creates a new EventManager using the specified EventQueue and Dispatcher.
         *
         * The EventManger takes onwership of the EventQueue and the Dispatcher.
         *
         * @param eventQueue
         * @param dispatcher
         */
        explicit EventManager(
            std::unique_ptr<EventQueue> eventQueue,
            std::unique_ptr<Dispatcher> dispatcher
        ) :
            eventQueue_(std::move(eventQueue)),
            dispatcher_(std::move(dispatcher))
        {}


        /**
         * @brief Posts  the event to the queue using the APPEND policy,
         * passing ownership of the event to the underlying queue.
         *
         * Calling this function is functionally identical to calling post()
         * with the second argument set to APPEND.
         *
         * @param event The event to post to the EventQueue.
         *
         * @return EventManager
         */
        EventManager& post(std::unique_ptr<const Event> event) {
            return post(std::move(event), APPEND, nullptr);
        }


        /**
         * @brief Posts  the event to the queue, passing ownership of the event
         * to the underlying queue.
         *
         * Calling this function is identical to calling post() with the third argument
         * func set to nullptr, relying on the default implementations.
         *
         * @param event The event to post to the EventQueue.
         * @param policy The policy to use for posting the event.
         *
         * @return EventManager
         */
        EventManager& post(std::unique_ptr<const Event> event, PostPolicy policy) {
            logger_.debug(std::format("Posting Event {0}", event->toString()));
            return post(std::move(event), policy, nullptr);
        }


        /**
         * @brief Posts the event to the underlying EventQueue using the specified
         * policy and the specified func.
         *
         * Implementing classes are responsible to treating func according to the
         * specified policy, e.g. APPEND for adding the event to the queue,
         * LATEST_WINS for removing a particular event in favor of the submitted
         * event.
         *
         * @param event A unique_ptr to the event to be posted. Will be the first argument
         * passed to func.
         * @param policy The post policy for the event (@see PostPolicy)
         * @param func The function used with the PostPolicy.
         * @return
         */
        virtual EventManager& post(
            std::unique_ptr<const Event> event,
            PostPolicy policy,
            const std::function<bool(
                const std::unique_ptr<const Event>& event,
                const std::unique_ptr<const Event>& evt)>& func
        ) = 0;


        /**
         * @brief Dispatches all events currently held in the event queue.
         *
         * This method retrieves and removes events one by one from the underlying
         * `EventQueue`, passing ownership to the underlying Dispatcher.
         *
         * @return EventManager
         */
        virtual EventManager& dispatchAll() = 0;


        /**
         * @brief Subscribes a callback to the specified EventType.
         *
         * Delegates to the subscribe() method of the underlying dispatcher.
         *
         * @tparam EventType The specific tyoe of the event to subscribe to.
         * @param callback The callback function that should be called when an event of the
         * specific type is triggered.
         *
         * @return EventManager
         */
        template<typename EventType>
        EventManager& subscribe(std::function<void(const EventType&)> callback) {
            dispatcher_->subscribe(callback);

            return *this;
        }


        /**
         * @brief Returns a reference to the dispatcher used with this EventManager.
         *
         * Provides direct access to the dispatcher, allowing for modification
         * of the Dispatcher, e.g. managing subscriptions.
         *
         * @return Dispatcher
         */
        [[nodiscard]] Dispatcher& dispatcher() const noexcept {
            return *dispatcher_;
        }


    };

}