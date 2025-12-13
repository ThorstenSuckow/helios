#include <gtest/gtest.h>

import helios.core.units;

using namespace helios::core::units;

TEST(UnitsTest, Constants) {
    EXPECT_FLOAT_EQ(CENTIMETERS, 0.01f);
    EXPECT_FLOAT_EQ(METERS, 1.0f);
    EXPECT_FLOAT_EQ(MILLISECONDS, 0.001f);
    EXPECT_FLOAT_EQ(SECONDS, 1.0f);
}

TEST(UnitsTest, SpatialConversion) {
    // 100 cm = 1 m
    EXPECT_FLOAT_EQ(fromCm(100.0f), 1.0f);
    EXPECT_FLOAT_EQ(fromCm(50.0f), 0.5f);

    // 1 m = 1 m
    EXPECT_FLOAT_EQ(fromM(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(fromM(1.5f), 1.5f);
}

TEST(UnitsTest, TemporalConversion) {
    // 1000 ms = 1 s
    EXPECT_FLOAT_EQ(fromMs(1000.0f), 1.0f);
    EXPECT_FLOAT_EQ(fromMs(500.0f), 0.5f);

    // 1 s = 1 s
    EXPECT_FLOAT_EQ(fromS(1.0f), 1.0f);
    EXPECT_FLOAT_EQ(fromS(2.5f), 2.5f);
}

TEST(UnitsTest, GenericConversion) {
    // Spatial
    EXPECT_FLOAT_EQ(from(100.0f, Unit::Centimeter), 1.0f);
    EXPECT_FLOAT_EQ(from(1.0f, Unit::Meter), 1.0f);

    // Temporal
    EXPECT_FLOAT_EQ(from(1000.0f, Unit::MilliSeconds), 1.0f);
    EXPECT_FLOAT_EQ(from(1.0f, Unit::Seconds), 1.0f);
}

TEST(UnitsTest, ZeroValues) {
    EXPECT_FLOAT_EQ(fromCm(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(fromM(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(fromMs(0.0f), 0.0f);
    EXPECT_FLOAT_EQ(fromS(0.0f), 0.0f);
}

TEST(UnitsTest, NegativeValues) {
    EXPECT_FLOAT_EQ(fromCm(-100.0f), -1.0f);
    EXPECT_FLOAT_EQ(fromMs(-1000.0f), -1.0f);
}

