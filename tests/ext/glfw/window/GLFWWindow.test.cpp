#include <gtest/gtest.h>
#include <type_traits>

import helios.window;
import helios.ext.glfw.window.GLFWWindow;
import helios.ext.glfw.window.GLFWWindowConfig;

using namespace helios::window;
using namespace helios::ext::glfw::window;



TEST(GLFWWindowConfigTest, data) {
    EXPECT_TRUE((std::is_base_of_v<WindowConfig, GLFWWindowConfig>));
    EXPECT_TRUE((std::is_convertible_v<GLFWWindowConfig*, WindowConfig*>));

    const auto cfg = std::make_unique<GLFWWindowConfig>();

    EXPECT_EQ(800, cfg->height);
    EXPECT_EQ(800, cfg->width);
    EXPECT_FALSE(cfg->frameBufferSizeCallback);
    EXPECT_EQ(std::string{"helios - Window"}, cfg->title);
}


