#include <gtest/gtest.h>
#include <type_traits>


import helios.event;
import helios.util;


using namespace helios::event::core;
using namespace helios::event;

class TestEvent : public Event {
public:
    TestEvent() :Event() {}
    TestEvent(uint64_t tag): Event(tag) {}
};

TEST(EventManagerTest, dispatchAll) {

    auto e = std::make_unique<TestEvent>();

    auto eventManager = BasicEventManager(
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


TEST(PostTest, uniqueevent) {

    const uint64_t tag = helios::util::Guid::generate().value();

    auto e1 = std::make_unique<TestEvent>(tag);
    auto e2 = std::make_unique<TestEvent>(tag);
    auto e3 = std::make_unique<TestEvent>(tag);

    auto eventManager = BasicEventManager(
    std::make_unique<EventQueue>(),
    std::make_unique<Dispatcher>()
    );

    int result = 0;
    eventManager.subscribe<TestEvent>([&result](const TestEvent& e) {
        result++;
    });

    eventManager.post(std::move(e1), PostPolicy::LATEST_WINS);
    eventManager.post(std::move(e2), PostPolicy::LATEST_WINS);
    eventManager.post(std::move(e3), PostPolicy::LATEST_WINS);

    eventManager.dispatchAll();

    EXPECT_EQ(result, 1);

}
