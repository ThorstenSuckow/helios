module;

#include <functional>
#include <memory>

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
           std::unique_ptr<Dispatcher> dispatcher);


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
        ) override;


        /**
         * @brief Dispatches all events of the queue, passing ownership to the
         * underlying Dispatcher.
         * Clears the underlying EventQueue.
         *
         * @return EventManager
         */
        EventManager& dispatchAll() override;



    };

}