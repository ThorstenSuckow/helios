#include <gtest/gtest.h>
#include <type_traits>

import helios.platform.window.core;
import helios.platform.window.glfw;

using namespace helios::platform::window::core;
using namespace helios::platform::window::glfw;



TEST(GLFWWindowConfigTest, data) {
    EXPECT_TRUE((std::is_base_of_v<WindowConfig, GLFWWindowConfig>));
    EXPECT_TRUE((std::is_convertible_v<GLFWWindowConfig*, WindowConfig*>));

    const auto cfg = std::make_unique<GLFWWindowConfig>();

    EXPECT_EQ(600, cfg->height);
    EXPECT_EQ(800, cfg->width);
    EXPECT_FALSE(cfg->frameBufferSizeCallback);
    EXPECT_EQ(std::string{"helios - Window"}, cfg->title);
}


