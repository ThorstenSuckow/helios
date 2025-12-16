#include <gtest/gtest.h>

import helios.input.gamepad.GamepadSettings;

using namespace helios::input::gamepad;

TEST(GamepadSettingsTest, DefaultConstructorInitializesZeroDeadzoneAndNoInversion) {
    GamepadSettings settings;

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.0f);
    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.0f);
    EXPECT_FALSE(settings.invertLeftX());
    EXPECT_FALSE(settings.invertLeftY());
    EXPECT_FALSE(settings.invertRightX());
    EXPECT_FALSE(settings.invertRightY());
}

TEST(GamepadSettingsTest, SetLeftStickDeadzoneReturnsCorrectValue) {
    GamepadSettings settings;
    settings.setLeftStickDeadzone(0.15f);

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.15f);
}

TEST(GamepadSettingsTest, SetRightStickDeadzoneReturnsCorrectValue) {
    GamepadSettings settings;
    settings.setRightStickDeadzone(0.2f);

    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.2f);
}

TEST(GamepadSettingsTest, DeadzoneClampedToMinimumZero) {
    GamepadSettings settings;
    settings.setLeftStickDeadzone(-0.5f);
    settings.setRightStickDeadzone(-1.0f);

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.0f);
    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.0f);
}

TEST(GamepadSettingsTest, DeadzoneClampedToMaximum09) {
    GamepadSettings settings;
    settings.setLeftStickDeadzone(1.0f);
    settings.setRightStickDeadzone(0.95f);

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.9f);
    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.9f);
}

TEST(GamepadSettingsTest, DeadzoneBoundaryValues) {
    GamepadSettings settings;

    settings.setLeftStickDeadzone(0.0f);
    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.0f);

    settings.setLeftStickDeadzone(0.9f);
    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.9f);
}

TEST(GamepadSettingsTest, SetInvertLeftXReturnsCorrectValue) {
    GamepadSettings settings;

    settings.setInvertLeftX(true);
    EXPECT_TRUE(settings.invertLeftX());

    settings.setInvertLeftX(false);
    EXPECT_FALSE(settings.invertLeftX());
}

TEST(GamepadSettingsTest, SetInvertLeftYReturnsCorrectValue) {
    GamepadSettings settings;

    settings.setInvertLeftY(true);
    EXPECT_TRUE(settings.invertLeftY());

    settings.setInvertLeftY(false);
    EXPECT_FALSE(settings.invertLeftY());
}

TEST(GamepadSettingsTest, SetInvertRightXReturnsCorrectValue) {
    GamepadSettings settings;

    settings.setInvertRightX(true);
    EXPECT_TRUE(settings.invertRightX());

    settings.setInvertRightX(false);
    EXPECT_FALSE(settings.invertRightX());
}

TEST(GamepadSettingsTest, SetInvertRightYReturnsCorrectValue) {
    GamepadSettings settings;

    settings.setInvertRightY(true);
    EXPECT_TRUE(settings.invertRightY());

    settings.setInvertRightY(false);
    EXPECT_FALSE(settings.invertRightY());
}

TEST(GamepadSettingsTest, FluentSetterChainingWorks) {
    GamepadSettings settings;

    settings.setLeftStickDeadzone(0.1f)
            .setRightStickDeadzone(0.2f)
            .setInvertLeftX(true)
            .setInvertLeftY(true)
            .setInvertRightX(false)
            .setInvertRightY(true);

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.1f);
    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.2f);
    EXPECT_TRUE(settings.invertLeftX());
    EXPECT_TRUE(settings.invertLeftY());
    EXPECT_FALSE(settings.invertRightX());
    EXPECT_TRUE(settings.invertRightY());
}

TEST(GamepadSettingsTest, IndependentAxisInversionSettings) {
    GamepadSettings settings;

    settings.setInvertLeftX(true);
    settings.setInvertRightY(true);

    EXPECT_TRUE(settings.invertLeftX());
    EXPECT_FALSE(settings.invertLeftY());
    EXPECT_FALSE(settings.invertRightX());
    EXPECT_TRUE(settings.invertRightY());
}

TEST(GamepadSettingsTest, IndependentDeadzoneSettings) {
    GamepadSettings settings;

    settings.setLeftStickDeadzone(0.3f);

    EXPECT_FLOAT_EQ(settings.leftStickDeadzone(), 0.3f);
    EXPECT_FLOAT_EQ(settings.rightStickDeadzone(), 0.0f);
}

