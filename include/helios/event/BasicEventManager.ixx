/**
 * @file BasicEventManager.ixx
 * @brief Default implementation of an EventManager.
 */
module;

#include <functional>
#include <memory>
#include <utility>

export module helios.event.BasicEventManager;

import helios.event.EventManager;
import helios.event.EventQueue;
import helios.event.Dispatcher;
import helios.event.Event;

using namespace helios::event;

export namespace helios::event {


    /**
     * @brief Basic implementation of the EventManager.
     *
     * This implementation provides the concrete logic for
     * post() and dispatchAll().
     */
    class BasicEventManager final : public EventManager {

    public:
        using EventManager::post;

        /**
         * @brief Constructs a new BasicEventManager, using the specified EventQueue
         * and the Dispatcher.
         *
         * @param eventQueue
         * @param dispatcher
         */
        explicit BasicEventManager(std::unique_ptr<EventQueue> eventQueue,
           std::unique_ptr<Dispatcher> dispatcher) :
           EventManager(std::move(eventQueue), std::move(dispatcher))
        {}


        /**
         * @brief Posts an event based on the specified policy to the EventQueue.
         *
         * @param event A unique_ptr to the event that should be posted
         * @param policy The policy to use for positing the event.
         * @param func A comparison function for LATEST_WINS policy. If none is specified,
         * this method will compare the events based on type and tag equality.
         *
         * @return EventManager
         */
        EventManager& post(
            std::unique_ptr<const Event> event,
            PostPolicy policy,
            const std::function<bool(
                const std::unique_ptr<const Event>& event,
                const std::unique_ptr<const Event>& evt)>& func
        ) override {

            std::function<bool(const std::unique_ptr<const Event>& event,
                                    const std::unique_ptr<const Event>& evt)> cmpFunc;

            switch (policy) {
                case APPEND:
                    eventQueue_->add(std::move(event));
                break;
                case LATEST_WINS:

                    if (!func) {
                        cmpFunc = [](
                            const std::unique_ptr<const Event>& event,
                            const std::unique_ptr<const Event>& evt) {
                                if (!event  || !evt) {
                                    return false;
                                }
                                return typeid(*event) == typeid(*evt) && event->tag() == evt->tag();
                        };
                    } else {
                        cmpFunc = func;
                    }

                    /**
                     * @todo use hashmap instead of queue for faster
                     * lookup
                     */
                    eventQueue_->addOrReplace(std::move(event), cmpFunc);
                    break;
                default:
                    std::unreachable();
            }


            return *this;
        }


        /**
         * @brief Dispatches all events of the queue, passing ownership to the
         * underlying Dispatcher.
         * Clears the underlying EventQueue.
         *
         * @return EventManager
         */
        EventManager& dispatchAll() override {
            while (!eventQueue_->empty()) {
                auto e = eventQueue_->next();
                dispatcher_->dispatch(std::move(e));
            }

            return *this;
        }



    };

}