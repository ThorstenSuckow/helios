#include <gtest/gtest.h>

import helios.input.gamepad.GamepadState;
import helios.math.types;

using namespace helios::input;
using namespace helios::input::gamepad;
using namespace helios::math;

TEST(GamepadStateTest, DefaultConstructionInitializesToZeroAndButtonsFalse) {
    GamepadState s;

    EXPECT_FLOAT_EQ(s.leftX(), 0.0f);
    EXPECT_FLOAT_EQ(s.leftY(), 0.0f);
    EXPECT_FLOAT_EQ(s.rightX(), 0.0f);
    EXPECT_FLOAT_EQ(s.rightY(), 0.0f);
    EXPECT_FLOAT_EQ(s.triggerLeft(), 0.0f);
    EXPECT_FLOAT_EQ(s.triggerRight(), 0.0f);

    EXPECT_FALSE(s.buttonA());
    EXPECT_FALSE(s.buttonB());
    EXPECT_FALSE(s.buttonX());
    EXPECT_FALSE(s.buttonY());
    EXPECT_FALSE(s.buttonStart());
    EXPECT_FALSE(s.buttonBack());
    EXPECT_FALSE(s.buttonGuide());
    EXPECT_FALSE(s.buttonLeftBumper());
    EXPECT_FALSE(s.buttonRightBumper());
    EXPECT_FALSE(s.buttonLeftThumb());
    EXPECT_FALSE(s.buttonRightThumb());
    EXPECT_FALSE(s.buttonDpadUp());
    EXPECT_FALSE(s.buttonDpadRight());
    EXPECT_FALSE(s.buttonDpadDown());
    EXPECT_FALSE(s.buttonDpadLeft());
}

TEST(GamepadStateTest, UpdateAxesSetsValuesAndButtons) {
    GamepadState s;

    s.updateAxes(0.5f, -0.5f, // left x,y
                 1.0f, -1.0f, // right x,y
                 0.25f, 0.75f, // triggers
                 true, false, // A, B
                 true, false, // X, Y
                 true, false, // Start, Back
                 true, // Guide
                 true, false, // left/right bumper
                 true, false, // left/right thumb
                 true, false, true, false // dpad up/right/down/left
    );

    EXPECT_FLOAT_EQ(s.leftX(), 0.5f);
    EXPECT_FLOAT_EQ(s.leftY(), -0.5f);
    EXPECT_FLOAT_EQ(s.rightX(), 1.0f);
    EXPECT_FLOAT_EQ(s.rightY(), -1.0f);
    EXPECT_FLOAT_EQ(s.triggerLeft(), 0.25f);
    EXPECT_FLOAT_EQ(s.triggerRight(), 0.75f);

    auto l = s.left();
    EXPECT_FLOAT_EQ(l[0], 0.5f);
    EXPECT_FLOAT_EQ(l[1], -0.5f);

    auto t = s.trigger();
    EXPECT_FLOAT_EQ(t[0], 0.25f);
    EXPECT_FLOAT_EQ(t[1], 0.75f);

    EXPECT_TRUE(s.buttonA());
    EXPECT_FALSE(s.buttonB());
    EXPECT_TRUE(s.buttonX());
    EXPECT_FALSE(s.buttonY());
    EXPECT_TRUE(s.buttonPressedStart());
    EXPECT_FALSE(s.buttonBack());
    EXPECT_TRUE(s.buttonGuide());
    EXPECT_TRUE(s.buttonLeftBumper());
    EXPECT_FALSE(s.buttonRightBumper());
    EXPECT_TRUE(s.buttonLeftThumb());
    EXPECT_FALSE(s.buttonRightThumb());
    EXPECT_TRUE(s.buttonDpadUp());
    EXPECT_FALSE(s.buttonDpadRight());
    EXPECT_TRUE(s.buttonDpadDown());
    EXPECT_FALSE(s.buttonDpadLeft());
}

TEST(GamepadStateTest, UpdateAxesClampsOutOfRangeValues) {
    GamepadState s;

    s.updateAxes(2.0f, -2.0f, // left x,y out of range
                 1.5f, -1.5f, // right x,y out of range
                 -0.5f, 2.0f, // triggers out of range
                 false, false, false, false, false, false, false, false, false, false, false, false, false, false,
                 false);

    EXPECT_FLOAT_EQ(s.leftX(), 1.0f);
    EXPECT_FLOAT_EQ(s.leftY(), -1.0f);
    EXPECT_FLOAT_EQ(s.rightX(), 1.0f);
    EXPECT_FLOAT_EQ(s.rightY(), -1.0f);
    EXPECT_FLOAT_EQ(s.triggerLeft(), 0.0f);
    EXPECT_FLOAT_EQ(s.triggerRight(), 1.0f);
}

TEST(GamepadStateTest, ConstructorInitializesValuesAndButtons) {
    GamepadState s(0.1f, 0.2f, -0.3f, 0.4f, 0.0f, 1.0f, true, true, false, false, // A,B,X,Y
                   false, true, true, false, // Start,Back,Guide,leftBumper
                   true, false, true, false, // rightBumper,leftThumb,rightThumb,dpadUp
                   false, false, false // dpadRight, dpadDown, dpadLeft (fill to match param order)
    );

    EXPECT_FLOAT_EQ(s.leftX(), 0.1f);
    EXPECT_FLOAT_EQ(s.leftY(), 0.2f);
    EXPECT_FLOAT_EQ(s.rightX(), -0.3f);
    EXPECT_FLOAT_EQ(s.rightY(), 0.4f);
    EXPECT_FLOAT_EQ(s.triggerLeft(), 0.0f);
    EXPECT_FLOAT_EQ(s.triggerRight(), 1.0f);

    EXPECT_TRUE(s.buttonA());
    EXPECT_TRUE(s.buttonB());
    EXPECT_FALSE(s.buttonX());
    EXPECT_FALSE(s.buttonY());
}

TEST(GamepadStateTest, CachedVec2fReflectsLatestUpdate) {
    GamepadState s;

    s.updateAxes(0.3f, 0.4f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, false, false, false, false, false, false,
                 false, false, false, false, false, false);
    auto v1 = s.left();

    s.updateAxes(-0.6f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, false, false, false, false, false, false, false, false, false,
                 false, false, false, false, false, false);
    auto v2 = s.left();

    EXPECT_FLOAT_EQ(v1[0], 0.3f);
    EXPECT_FLOAT_EQ(v1[1], 0.4f);
    EXPECT_FLOAT_EQ(v2[0], -0.6f);
    EXPECT_FLOAT_EQ(v2[1], 0.7f);
}
