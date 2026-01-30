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

TEST(UniformSemanticsTests, TextTextureFollowsMaterialRoughness) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::TextTexture), 5);
}

TEST(UniformSemanticsTests, TextColorFollowsTextTexture) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::TextColor), 6);
}

TEST(UniformSemanticsTests, DeltaTimeFollowsTextColor) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::deltaTime), 7);
}

TEST(UniformSemanticsTests, TotalTimeFollowsDeltaTime) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::totalTime), 8);
}

TEST(UniformSemanticsTests, SizeEnumRepresentsNumberOfEntries) {
    EXPECT_EQ(static_cast<int>(UniformSemantics::size_), 9);
}