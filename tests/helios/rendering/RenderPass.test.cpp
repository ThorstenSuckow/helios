#include <gtest/gtest.h>
#include <memory>

import helios.rendering;

using namespace helios::rendering;
using namespace helios::rendering::shader;

TEST(RenderPassTest, HandlesArgsProperly) {

    auto frameUniformValues = UniformValueMap();
    frameUniformValues.set(UniformSemantics::MaterialRoughness, 1.0f);

    auto objectUniformValues = UniformValueMap();
    auto materialUniformValues = UniformValueMap();

    auto renderCommand = mesh::MeshRenderCommand(nullptr, objectUniformValues, materialUniformValues);

    auto queue = RenderQueue();
    queue.add(std::move(renderCommand));

    auto viewport = helios::rendering::Viewport(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(&viewport, std::move(queue), frameUniformValues);

    EXPECT_EQ(pass.renderQueue().meshRenderCommandsSize(), 1);
    const float* roughness = pass.frameUniformValues().float_ptr(UniformSemantics::MaterialRoughness);
    ASSERT_NE(roughness, nullptr);
    EXPECT_EQ(*roughness, 1.0f);
}


// --------------------
// @see helios/#74
// --------------------
TEST(RenderPassTest, HandlesEmptyRenderQueueGracefully) {
    auto uniformValues = UniformValueMap();
    auto viewport = helios::rendering::Viewport(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(&viewport, RenderQueue(), uniformValues);
    EXPECT_EQ(pass.renderQueue().meshRenderCommandsSize(), 0);
}

TEST(RenderPassTest, HandlesEmptyUniformValueMapGracefully) {
    auto renderQueue = RenderQueue();
    auto viewport = helios::rendering::Viewport(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(&viewport, std::move(renderQueue), UniformValueMap());

    EXPECT_EQ(pass.frameUniformValues().mat4f_ptr(UniformSemantics::ModelMatrix), nullptr);
}

TEST(RenderPassTest, HandlesBothEmptyArgsGracefully) {
    auto viewport = helios::rendering::Viewport(0.0f, 0.0f, 1.0f, 1.0f);

    auto pass = RenderPass(&viewport, RenderQueue(), UniformValueMap());

    EXPECT_EQ(pass.renderQueue().meshRenderCommandsSize(), 0);
    EXPECT_EQ(pass.frameUniformValues().mat4f_ptr(UniformSemantics::ModelMatrix), nullptr);
}
// --------------------
// --------------------


// --------------------
// @see helios/#
// --------------------
TEST(RenderPassTest, HandlesNullViewportE) {
    auto uniformValues = UniformValueMap();

    #ifdef HELIOS_DEBUG
        EXPECT_DEATH(
                RenderPass(nullptr, RenderQueue(), uniformValues),
                ".*Unexpected nullptr.*"
        );
    #endif
}
