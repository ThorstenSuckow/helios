#include <gtest/gtest.h>
#include <type_traits>

import helios.platform.core;
import helios.platform.glfw;

namespace platform = helios::platform;
namespace glfw = platform::glfw;



TEST(GLFWWindowConfigTest, data) {
    EXPECT_TRUE((std::is_base_of_v<platform::WindowConfig, glfw::GLFWWindowConfig>));
    EXPECT_TRUE((std::is_convertible_v<glfw::GLFWWindowConfig*, platform::WindowConfig*>));

    const auto cfg = std::make_unique<glfw::GLFWWindowConfig>();

    EXPECT_EQ(600, cfg->height);
    EXPECT_EQ(800, cfg->width);
    EXPECT_EQ(std::string{"helios - Window"}, cfg->title);
}


