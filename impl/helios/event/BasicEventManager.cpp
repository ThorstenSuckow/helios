module;

#include <functional>
#include <memory>
#include <utility>

module helios.event.BasicEventManager;

import helios.event.EventManager;

using namespace helios::event;

namespace helios::event {


    BasicEventManager::BasicEventManager(std::unique_ptr<EventQueue> eventQueue,
               std::unique_ptr<Dispatcher> dispatcher) :
               EventManager(std::move(eventQueue), std::move(dispatcher))
    {}

    EventManager& BasicEventManager::post(
        std::unique_ptr<const Event> event,
        const PostPolicy policy,
        const std::function
            <bool(
            const std::unique_ptr<const Event>& event,
            const std::unique_ptr<const Event>& evt
            )>& func) {

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


    EventManager& BasicEventManager::dispatchAll() {
        while (!eventQueue_->empty()) {
            auto e = eventQueue_->next();
            dispatcher_->dispatch(std::move(e));
        }

        return *this;
    }




};

