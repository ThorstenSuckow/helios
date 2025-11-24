#include <gtest/gtest.h>
#include <memory>

import helios.rendering.RenderPass;
import helios.rendering.RenderQueue;
import helios.rendering.RenderCommand;
import helios.rendering.Viewport;
import helios.rendering.ClearFlags;
import helios.rendering.RenderPrototype;
import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

using namespace helios::rendering;
using namespace helios::rendering::shader;

TEST(RenderPassTest, HandlesArgsProperly) {

    auto frameUniformValues = std::make_unique<UniformValueMap>();
    frameUniformValues->set(UniformSemantics::MaterialRoughness, 1.0f);

    auto objectUniformValues = std::make_unique<UniformValueMap>();
    std::weak_ptr<RenderPrototype> weak;
    auto renderCommand = std::make_unique<RenderCommand>(
        weak,
        std::move(objectUniformValues),
        nullptr
    );

    auto queue = std::make_unique<RenderQueue>();
    queue->add(std::move(renderCommand));

    const auto viewport = std::make_shared<const helios::rendering::Viewport>(
        0.0f, 0.0f, 1.0f, 1.0f
    );

    auto pass = RenderPass(viewport, std::move(queue), std::move(frameUniformValues));

    EXPECT_EQ(pass.renderQueue().count(), 1);
    const float* roughness = pass.frameUniformValues()
                                 .float_ptr(UniformSemantics::MaterialRoughness);
    ASSERT_NE(roughness, nullptr);
    EXPECT_EQ(*roughness, 1.0f);

}


// --------------------
// @see helios/#74
// --------------------
TEST(RenderPassTest, HandlesNullRenderQueueGracefully) {
    auto uniformValues = std::make_unique<UniformValueMap>();
    const auto viewport = std::make_shared<const helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(viewport, nullptr, std::move(uniformValues));
    EXPECT_EQ(pass.renderQueue().count(), 0);
}

TEST(RenderPassTest, HandlesNullUniformValueMapGracefully) {
    auto renderQueue = std::make_unique<RenderQueue>();
    const auto viewport = std::make_shared<const helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(viewport, std::move(renderQueue), nullptr);

    EXPECT_EQ(pass.frameUniformValues().mat4f_ptr(UniformSemantics::WorldMatrix), nullptr);
}

TEST(RenderPassTest, HandlesBothNullArgsGracefully) {
    const auto viewport = std::make_shared<const helios::rendering::Viewport>(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(viewport, nullptr, nullptr);

    EXPECT_EQ(pass.renderQueue().count(), 0);
    EXPECT_EQ(pass.frameUniformValues().mat4f_ptr(UniformSemantics::WorldMatrix), nullptr);
}
// --------------------
// --------------------


// --------------------
// @see helios/#
// --------------------
TEST(RenderPassTest, HandlesNullViewportGracefully) {
    auto uniformValues = std::make_unique<UniformValueMap>();

    auto pass = RenderPass(nullptr, nullptr, std::move(uniformValues));
    EXPECT_EQ(pass.viewport().clearFlags(), std::to_underlying(ClearFlags::None));
}
// --------------------
// --------------------