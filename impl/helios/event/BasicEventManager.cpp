module;

#include <memory>
#include <utility>

module helios.event;

import helios.event;

using namespace helios::event;

namespace helios::event {

    EventManager& BasicEventManager::post(
        std::unique_ptr<const Event> e,
        const PostPolicy policy,
        const std::function<bool(
            const std::unique_ptr<const Event>& evt,
            const std::unique_ptr<const Event>& e)>& func) {

        std::function<bool(const std::unique_ptr<const Event>& evt,
                                const std::unique_ptr<const Event>& e)> cmpFunc;

        switch (policy) {
            case APPEND:
                eventQueue_->add(std::move(e));
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
                eventQueue_->addOrReplace(std::move(e), cmpFunc);
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

