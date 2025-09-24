module;

#include <memory>
#include <utility>

module helios.event;

import helios.event.core;

using namespace helios::event::core;

namespace helios::event {

    EventManager& EventManager::post(
        std::unique_ptr<const Event> e,
        const PostPolicy policy,
        const std::function<bool(
            const std::unique_ptr<const Event>& evt,
            const std::unique_ptr<const Event>& e)>& func) {

        std::function<bool(const std::unique_ptr<const Event>& evt,
                                const std::unique_ptr<const Event>& e)> cmpFunc;

        switch (policy) {
            case APPEND:
                eventQueue_->push(std::move(e));
            break;
            case LATEST_WINS:

                if (!func) {
                    cmpFunc = [](
                        const std::unique_ptr<const Event>& evt,
                        const std::unique_ptr<const Event>& e) {
                            if (!evt  || !e) {
                                return false;
                            }
                            return typeid(*evt) == typeid(*e) && evt->tag() == e->tag();
                    };
                } else {
                    cmpFunc = func;
                }

                /**
                 * @todo use hashmap instead of queue for faster
                 * lookup
                 */
                eventQueue_->pushOrReplace(std::move(e), cmpFunc);
                break;
            default:
                std::unreachable();
        }


        return *this;
    }


    EventManager& EventManager::dispatchAll() {
        while (!eventQueue_->empty()) {
            auto e = eventQueue_->popFront();
            dispatcher_->dispatch(std::move(e));
        }

        return *this;
    }




};

