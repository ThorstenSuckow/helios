#include <gtest/gtest.h>

import helios.rendering.shader.UniformValueMap;
import helios.rendering.shader.UniformSemantics;

using namespace helios::rendering::shader;

TEST(UniformValueMap, HandlesFloatValuesCorrectly) {

    auto map = UniformValueMap();

    map.set(UniformSemantics::MaterialRoughness, 1.0f);

    const float* roughness = map.float_ptr(UniformSemantics::MaterialRoughness);

    ASSERT_NE(roughness, nullptr);

    EXPECT_FLOAT_EQ(*roughness, 1.0f);
}