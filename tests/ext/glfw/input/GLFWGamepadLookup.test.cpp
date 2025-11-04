#include <gtest/gtest.h>
#include <GLFW/glfw3.h>


import helios.ext.glfw.input.GLFWGamepadLookup;
import helios.input.types.Gamepad;

using namespace helios::input::types;
using namespace helios::ext::glfw::input;



TEST(GLFWGamepadLookupTest, from) {
    EXPECT_EQ(GLFWGamepadLookup::from(Gamepad::ONE), GLFW_JOYSTICK_1);
    EXPECT_EQ(GLFWGamepadLookup::from(Gamepad::TWO), GLFW_JOYSTICK_2);
    EXPECT_EQ(GLFWGamepadLookup::from(Gamepad::THREE), GLFW_JOYSTICK_3);
    EXPECT_EQ(GLFWGamepadLookup::from(Gamepad::FOUR), GLFW_JOYSTICK_4);
}

TEST(GLFWGamepadLookupTest, toArrayIndex) {
    EXPECT_EQ(GLFWGamepadLookup::toArrayIndex(Gamepad::ONE), 0);
    EXPECT_EQ(GLFWGamepadLookup::toArrayIndex(Gamepad::TWO), 1);
    EXPECT_EQ(GLFWGamepadLookup::toArrayIndex(Gamepad::THREE), 2);
    EXPECT_EQ(GLFWGamepadLookup::toArrayIndex(Gamepad::FOUR), 3);
}

