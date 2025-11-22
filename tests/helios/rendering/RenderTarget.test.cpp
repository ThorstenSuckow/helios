#include <gtest/gtest.h>
#include <memory>


import helios.rendering.RenderTarget;
import helios.rendering.Viewport;


TEST(RenderTargetTest, ViewportIsNullptr) {

    auto viewport = std::make_shared<helios::rendering::Viewport>();
    auto renderTarget = helios::rendering::RenderTarget(viewport, 10, 10);

    EXPECT_ANY_THROW(renderTarget.addViewport(nullptr));
}