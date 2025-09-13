#include <gtest/gtest.h>


import helios.platform.window.core;


using namespace helios::platform::window::core;

struct test_data {

};

static test_data setup() {
    return test_data{};
}


TEST(WindowConfigTest, data) {

    // init data
    const WindowConfig data{};

    EXPECT_EQ(600, data.height);
    EXPECT_EQ(800, data.width);
    EXPECT_STREQ("helios - Window", data.title.c_str());
}


