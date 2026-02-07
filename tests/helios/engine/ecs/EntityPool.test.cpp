#include <gtest/gtest.h>
#include <helios/helios_config.h>

import helios.engine.ecs;

using namespace helios::engine::ecs;

struct Entity {
    size_t id;
};



TEST(EntityPoolTest, addAndGet) {

    EntityPool<Entity> entityPool;

    auto one = std::make_unique<Entity>();
    auto handleOne = entityPool.emplace(std::move(one));

    auto two = std::make_unique<Entity>(2);
    auto handleTwo = entityPool.emplace(std::move(two));

    EXPECT_EQ(handleOne.entityId, 0);
    EXPECT_EQ(handleOne.versionId, 1);
    EXPECT_EQ(handleTwo.entityId, 1);
    EXPECT_EQ(handleTwo.versionId, 1);

}
