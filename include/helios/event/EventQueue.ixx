module;

#include <deque>
#include <functional>
#include <memory>

export module helios.event.EventQueue;

import helios.event.Event;

export namespace helios::event {

    /**
     * @brief Abstract base class defining an interface for an EventQueue.
     *
     * The `EventQueue` provides mechanism to buffer and retrieve events in
     * a structured manner. Implementing classes must define how events
     * are stored and processed.
     *
     * All events managed by this queue must derive from `Event`
     */
    class EventQueue {

    public:

        virtual ~EventQueue() = default;


        /**
         * @brief Adds a new event to the queue.
         * The queue will take ownership of the provided event.
         * The concrete position in the queue for the event will depend on the concrete
         * implementation.
         *
         * @param event A unique_ptr to the event this queue should buffer.
         *
         * @return EventQueue
         */
        virtual EventQueue& add(std::unique_ptr<const Event> event) = 0;


        /**
         * @brief Adds a new Event to the queue or replaces an existing one.
         *
         * This method allows for event management of events that should only be buffered
         * once, but are added to the queue from different sources in the application, such
         * that controlling the addition of events is not possible.
         *
         * Implementing classes ar responsible for treating the specified cmpFunc, provided
         * by the caller.
         *
         * @param event A unique_ptr to a new or existing event that should be replaced.
         * @param cmpFunc A compare function that provides the logic for looking up the event
         * that should be replaced with the specified event. The compFunc must accept
         * two event arguments, whereas the first argument is the Event to add, and the second
         * event is an existing event of the queue.
         *
         *
         * @return EventQueue
         */
        virtual EventQueue& addOrReplace(
            std::unique_ptr<const Event> event,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& cmpFunc) = 0;

        /**
         * @brief Returns true if the queue is empty, otherwise false.
         *
         * @return True if the queue is empty, otherwise false.
         */
        [[nodiscard]] virtual bool empty() const noexcept = 0;


        /**
         * @brief Retrieves and removes the "next" event in the queue.
         * Implementing classes are responsible for defining the meaning of "next",
         * i.e., FIFO, LIFO, ordered after a particular criteria etc.
         *
         * The ownership of the event is transferred to the caller.
         *
         * @return a unique_ptr to the next event.
         */
        virtual std::unique_ptr<const Event> next() = 0;

    };

}