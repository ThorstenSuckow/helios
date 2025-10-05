module;

#include <functional>
#include <memory>
#include <typeindex>

module helios.event.Dispatcher;

import helios.event.Event;


namespace helios::event {


    void Dispatcher::dispatch(std::unique_ptr<const Event> event) {
        const auto idx = std::type_index(typeid(*event));

        if (const auto cb = callbacks_.find(idx); cb != callbacks_.end()) {
            for (const auto& callback : cb->second) {
                callback(*event);
            }
        }
    }
};

