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


    class BasicEventManager : public EventManager {

    public:
        using EventManager::post;

        BasicEventManager(std::unique_ptr<EventQueue> eventQueue,
           std::unique_ptr<Dispatcher> dispatcher) :
           EventManager(std::move(eventQueue), std::move(dispatcher))
        {}



        EventManager& post(
            std::unique_ptr<const Event> e,
            PostPolicy policy,
            const std::function<bool(
                const std::unique_ptr<const Event>& evt,
                const std::unique_ptr<const Event>& e)>& func
        ) override;


        EventManager& dispatchAll() override;



    };

}