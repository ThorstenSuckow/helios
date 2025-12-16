/**
 * @file DeadzoneStrategy.test.cpp
 * @brief Unit tests for DeadzoneStrategy and RadialDeadzoneStrategy.
 */
#include <gtest/gtest.h>
#include <cmath>

import helios.input.gamepad.DeadzoneStrategy;
import helios.input.gamepad.RadialDeadzoneStrategy;

using namespace helios::input::gamepad;

// =============================================================================
// DeadzoneStrategy Tests (via RadialDeadzoneStrategy as concrete implementation)
// =============================================================================

TEST(DeadzoneStrategyTest, DefaultStickNoiseThresholdIsCorrect) {
    RadialDeadzoneStrategy strategy;

    EXPECT_FLOAT_EQ(strategy.stickNoiseThreshold(), DeadzoneStrategy::DEFAULT_STICK_NOISE_THRESHOLD);
}

TEST(DeadzoneStrategyTest, SetStickNoiseThresholdReturnsCorrectValue) {
    RadialDeadzoneStrategy strategy;
    strategy.setStickNoiseThreshold(0.05f);

    EXPECT_FLOAT_EQ(strategy.stickNoiseThreshold(), 0.05f);
}

TEST(DeadzoneStrategyTest, StickNoiseThresholdClampedToMinimumZero) {
    RadialDeadzoneStrategy strategy;
    strategy.setStickNoiseThreshold(-0.5f);

    EXPECT_FLOAT_EQ(strategy.stickNoiseThreshold(), 0.0f);
}

TEST(DeadzoneStrategyTest, StickNoiseThresholdClampedToMaximum09) {
    RadialDeadzoneStrategy strategy;
    strategy.setStickNoiseThreshold(1.0f);

    EXPECT_FLOAT_EQ(strategy.stickNoiseThreshold(), 0.9f);
}

TEST(DeadzoneStrategyTest, ConstructorWithCustomNoiseThreshold) {
    RadialDeadzoneStrategy strategy(0.1f);

    EXPECT_FLOAT_EQ(strategy.stickNoiseThreshold(), 0.1f);
}

// =============================================================================
// RadialDeadzoneStrategy Tests
// =============================================================================

TEST(RadialDeadzoneStrategyTest, InputWithinDeadzoneIsZeroed) {
    RadialDeadzoneStrategy strategy;
    float x = 0.1f;
    float y = 0.1f;

    strategy.normalize(0.2f, x, y);

    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, InputExactlyAtDeadzoneIsZeroed) {
    RadialDeadzoneStrategy strategy;
    float deadzone = 0.3f;
    float x = deadzone;
    float y = 0.0f;

    strategy.normalize(deadzone, x, y);

    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, InputBeyondDeadzoneIsRescaled) {
    RadialDeadzoneStrategy strategy;
    float deadzone = 0.2f;
    float x = 0.6f;
    float y = 0.0f;

    strategy.normalize(deadzone, x, y);

    // Expected: (0.6 - 0.2) / (1.0 - 0.2) = 0.4 / 0.8 = 0.5
    EXPECT_NEAR(x, 0.5f, 0.001f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, FullInputRemainsAtOne) {
    RadialDeadzoneStrategy strategy;
    float x = 1.0f;
    float y = 0.0f;

    strategy.normalize(0.2f, x, y);

    EXPECT_NEAR(x, 1.0f, 0.001f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, DiagonalInputIsNormalizedCorrectly) {
    RadialDeadzoneStrategy strategy;
    float deadzone = 0.1f;
    float x = 0.7f;
    float y = 0.7f;

    float originalMagnitude = std::hypot(x, y);
    strategy.normalize(deadzone, x, y);

    float resultMagnitude = std::hypot(x, y);
    float expectedMagnitude = (std::min(originalMagnitude, 1.0f) - deadzone) / (1.0f - deadzone);

    EXPECT_NEAR(resultMagnitude, expectedMagnitude, 0.001f);
}

TEST(RadialDeadzoneStrategyTest, InputExceedingUnitCircleIsClamped) {
    RadialDeadzoneStrategy strategy;
    float x = 1.5f;
    float y = 0.0f;

    strategy.normalize(0.1f, x, y);

    // Input clamped to 1.0, then rescaled: (1.0 - 0.1) / (1.0 - 0.1) = 1.0
    EXPECT_NEAR(x, 1.0f, 0.001f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, ZeroDeadzonePassesThroughInput) {
    RadialDeadzoneStrategy strategy;
    float x = 0.5f;
    float y = 0.3f;

    strategy.normalize(0.0f, x, y);

    EXPECT_NEAR(x, 0.5f, 0.001f);
    EXPECT_NEAR(y, 0.3f, 0.001f);
}

TEST(RadialDeadzoneStrategyTest, DeadzoneOfOneZeroesAllInput) {
    RadialDeadzoneStrategy strategy;
    float x = 0.9f;
    float y = 0.9f;

    strategy.normalize(1.0f, x, y);

    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, NegativeAxisValuesAreHandled) {
    RadialDeadzoneStrategy strategy;
    float x = -0.8f;
    float y = 0.0f;

    strategy.normalize(0.2f, x, y);

    // Input magnitude is 0.8, rescaled: (0.8 - 0.2) / 0.8 = 0.75
    EXPECT_NEAR(x, -0.75f, 0.001f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, NegativeDiagonalInputIsHandled) {
    RadialDeadzoneStrategy strategy;
    float x = -0.5f;
    float y = -0.5f;

    strategy.normalize(0.1f, x, y);

    float resultMagnitude = std::hypot(x, y);

    // Direction should be preserved (both negative)
    EXPECT_LT(x, 0.0f);
    EXPECT_LT(y, 0.0f);
    EXPECT_GT(resultMagnitude, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, InputBelowNoiseThresholdIsZeroed) {
    RadialDeadzoneStrategy strategy;
    strategy.setStickNoiseThreshold(0.1f);
    float x = 0.05f;
    float y = 0.05f;

    strategy.normalize(0.0f, x, y);

    EXPECT_FLOAT_EQ(x, 0.0f);
    EXPECT_FLOAT_EQ(y, 0.0f);
}

TEST(RadialDeadzoneStrategyTest, DirectionPreservedAfterNormalization) {
    RadialDeadzoneStrategy strategy;
    float x = 0.6f;
    float y = 0.8f;

    float originalX = x;
    float originalY = y;
    float originalRatio = originalX / originalY;

    strategy.normalize(0.2f, x, y);

    float resultRatio = x / y;
    EXPECT_NEAR(originalRatio, resultRatio, 0.001f);
}

