#include <gtest/gtest.h>


import helios.platform.core;

namespace platform = helios::platform;

struct test_data {

};

static test_data setup() {
    return test_data{};
}


TEST(WindowConfigTest, data) {

    // init data
    const platform::WindowConfig data{};

    EXPECT_EQ(600, data.height);
    EXPECT_EQ(800, data.width);
    EXPECT_STREQ("helios - Window", data.title.c_str());
}


