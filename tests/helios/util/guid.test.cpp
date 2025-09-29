#include <gtest/gtest.h>

import helios.util.Guid;

using Guid = helios::util::Guid;

TEST(Guid, generateAndValue) {

    // get initial value
    Guid start = Guid::generate();

    for (int i = 0; i < 10; i++) {
        Guid id = Guid::generate();
        EXPECT_EQ((i + 1) + start.value(), id.value());
    }
}

TEST(Guid, equals) {

    Guid l = Guid::generate();
    Guid r = Guid::generate();

    EXPECT_TRUE(l == l);
    EXPECT_TRUE(l != r);
    EXPECT_FALSE(l == r);
}
