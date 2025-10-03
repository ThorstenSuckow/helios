#include <gtest/gtest.h>
#include <type_traits>


import helios.event;

class TestEvent : public helios::event::Event {
};

TEST(DispatcherTest, dispatch) {

    auto dispatcher = helios::event::Dispatcher();
    std::string result;
    dispatcher.subscribe<TestEvent>([&result](const TestEvent& e) {
        result = "bar";
    });

    auto e = std::make_unique<TestEvent>();
    dispatcher.dispatch(std::move(e));

    EXPECT_EQ(result, "bar");

}


