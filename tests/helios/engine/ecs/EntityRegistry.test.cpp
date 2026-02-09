#include <gtest/gtest.h>
#include <helios/helios_config.h>

import helios.engine.ecs;
import helios.engine.core.data;

using namespace helios::engine::ecs;
using namespace helios::engine::core::data;


TEST(EntityRegistryTest, create) {

    EntityRegistry registry;
    {
        auto [entityId, versionId] = registry.create();

        EXPECT_EQ(entityId, 0);
        EXPECT_EQ(versionId, 1);
    }

    {
        auto [entityId, versionId] = registry.create();

        EXPECT_EQ(entityId, 1);
        EXPECT_EQ(versionId, 1);
    }
}

TEST(EntityRegistryTest, isValid) {

    EntityRegistry registry;

    auto handle = registry.create();

    EXPECT_TRUE(registry.isValid(handle));
    EXPECT_FALSE(registry.isValid({EntityId{2}, VersionId{1}}));

}


TEST(EntityRegistryTest, destroyAndreuse) {

    EntityRegistry registry;

    auto handle = registry.create();

    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);


    EXPECT_FALSE(registry.destroy({EntityId{1}, VersionId{2}}));
    EXPECT_TRUE(registry.destroy(handle));

    EXPECT_FALSE(registry.isValid(handle));

    handle = registry.create();

    // reused id, updated version
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 2);

}
