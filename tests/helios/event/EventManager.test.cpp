#include <gtest/gtest.h>
#include <type_traits>


import helios.event.core;

using namespace helios::event::core;

class TestEvent : public Event {
};

TEST(EventManagerTest, dispatchAll) {

    auto e = std::make_unique<TestEvent>();

    auto eventManager = EventManager(
    std::make_unique<EventQueue>(),
    std::make_unique<Dispatcher>()
    );

    std::string result;
    eventManager.subscribe<TestEvent>([&result](const TestEvent& e) {
        result = "bar";
    });

    eventManager.post(std::move(e));

    eventManager.dispatchAll();

    EXPECT_EQ(result, "bar");

}


