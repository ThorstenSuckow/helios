module;

#include <functional>
#include <memory>
#include <utility>

export module helios.event:EventManager;

import helios.event.core;

using namespace helios::event::core;

export namespace helios::event {


    enum PostPolicy {
        LATEST_WINS,
        APPEND,

        POLICY_COUNT
    };


    class EventManager {

    private:
        std::unique_ptr<EventQueue> eventQueue_;
        std::unique_ptr<Dispatcher> dispatcher_;

    public:
        EventManager(std::unique_ptr<EventQueue> eventQueue,
            std::unique_ptr<Dispatcher> dispatcher) :
            eventQueue_(std::move(eventQueue)),
            dispatcher_(std::move(dispatcher))
        {}


        EventManager& post(
            std::unique_ptr<const Event> e,
            PostPolicy policy=APPEND,
            const std::function<bool(const std::unique_ptr<const Event>& evt,
                                                    const std::unique_ptr<const Event>& e)>& func=nullptr);


        EventManager& dispatchAll();

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