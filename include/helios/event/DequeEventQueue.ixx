/**
 * @file DequeEventQueue.ixx
 * @brief Double-ended event queue implementation.
 */
module;

#include <deque>
#include <functional>
#include <memory>
#include <algorithm>

export module helios.event.DequeEventQueue;

import helios.event.EventQueue;
import helios.event.Event;


using namespace helios::event;

export namespace helios::event {

    /**
     * @brief Concrete implementation of an EventQueue that uses a Deque
     * as its underlying queue strategy.
     * Adding and removing events follows a FIFO strategy.
     *
     */
    class DequeEventQueue : public EventQueue{

    private:
        /**
         * @brief The internal deque for storing the events.
         */
        std::deque<std::unique_ptr<const Event>> events;

    public:

        /**
         * @brief Takes ownership of the event and moves it at the end of the queue.
         *
         * @param event A unique_ptr to the event that should be managed by this class.
         *
         * @return
         */
        EventQueue& add(std::unique_ptr<const Event> event) override {
            events.push_back(std::move(event));
            return *this;
        }


        /**
         * @brief Adds an event to the queue or replaces an existing one.
         *
         * @param event A unique_ptr to the event this queue takes the ownership of.
         * @param cmpFunc The cmp function that is used for looking up and replacing an existing
         * event with the soecified event.
         *
         * @return EventQueue
         */
        EventQueue& addOrReplace(
            std::unique_ptr<const Event> event,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& cmpFunc) override {

            std::erase_if(events, [&event, cmpFunc](const std::unique_ptr<const Event>& e) {
                return cmpFunc(event, e);
            });
            events.push_back(std::move(event));
            return *this;
        }


        /**
         * @brief Returns true if the queue is empty, otherwise false.
         *
         * @return True if the queue is empty, otherwise false.
         */
        [[nodiscard]] bool empty() const noexcept override {
            return events.empty();
        }


        /**
         * @brief Retrieves and returns the next event from the front of the queue.
         *
         * @return A unique_ptr to the next event in teh queue, i.e. from the front
         * position of the queue.
         */
        std::unique_ptr<const Event> next() override {
            if (events.empty()) {
                return nullptr;
            }

            auto event = std::move(events.front());
            events.pop_front();

            return event;
        }

    };

}