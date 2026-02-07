#include <gtest/gtest.h>
#include <helios/helios_config.h>


import helios.engine.ecs;
import helios.engine.core.data;

using namespace helios::engine::ecs;
using namespace helios::engine::core::data;

class Entity {

public:
    int value = 0;

    bool remove_ = true;

    bool onRemove() {
        return remove_;
    }

    bool operator==(const Entity& other) const {
        return value == other.value;
    }
};

class MyComponent {

    public:
    int value = 0;


};

TEST(EntityManager, create) {

    EntityRegistry registry;

    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);
}

TEST(EntityManager, remove) {

    EntityRegistry registry;
    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);

    EXPECT_TRUE(em.remove(handle));
}

TEST(EntityManager, emplace) {

    EntityRegistry registry;
    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);

    auto* cmp = em.emplace<MyComponent>(handle, 10);

    EXPECT_NE(cmp, nullptr);

    auto* ref = em.get<MyComponent>(handle);

    EXPECT_EQ(ref->value, 10);

    // ... and remove
    EXPECT_TRUE(em.remove(handle));

    EXPECT_EQ(em.get<MyComponent>(handle), nullptr);
}