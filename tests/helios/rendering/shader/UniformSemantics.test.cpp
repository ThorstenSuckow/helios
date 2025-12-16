#include <gtest/gtest.h>

import helios.rendering.shader.UniformSemantics;

using namespace helios::rendering::shader;

TEST(UniformSemanticsTests, ModelMatrixHasCorrectValue) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::ModelMatrix), 0);
}

TEST(UniformSemanticsTests, ProjectionMatrixFollowsModelMatrix) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::ProjectionMatrix), 1);
}

TEST(UniformSemanticsTests, ViewMatrixFollowsProjectionMatrix) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::ViewMatrix), 2);
}

TEST(UniformSemanticsTests, MaterialBaseColorFollowsViewMatrix) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::MaterialBaseColor), 3);
}

TEST(UniformSemanticsTests, MaterialRoughnessFollowsMaterialBaseColor) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::MaterialRoughness), 4);
}

TEST(UniformSemanticsTests, SizeEnumRepresentsNumberOfEntries) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::size_), 7);
}
