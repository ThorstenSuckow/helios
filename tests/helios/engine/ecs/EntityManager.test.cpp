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
    bool remove = true;

    bool onRemove() {
        return remove;
    }


};

TEST(EntityManager, create) {

    EntityRegistry registry;

    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);
}

TEST(EntityManager, destroy) {

    EntityRegistry registry;
    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);

    EXPECT_TRUE(em.destroy(handle));
}

TEST(EntityManager, emplace) {

    EntityRegistry registry;
    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);

    EXPECT_FALSE(em.has<MyComponent>(handle));

    auto* cmp = em.emplace<MyComponent>(handle, 10);

    EXPECT_TRUE(em.has<MyComponent>(handle));
    EXPECT_NE(cmp, nullptr);

    auto* ref = em.get<MyComponent>(handle);

    EXPECT_EQ(ref->value, 10);

    // ... and destroy
    EXPECT_TRUE(em.destroy(handle));

    EXPECT_EQ(em.get<MyComponent>(handle), nullptr);
}

TEST(EntityManager, remove) {

    EntityRegistry registry;
    EntityManager em(registry);

    const auto handle = em.create();
    EXPECT_EQ(handle.entityId, 0);
    EXPECT_EQ(handle.versionId, 1);

    EXPECT_FALSE(em.has<MyComponent>(handle));

    auto* cmp = em.emplace<MyComponent>(handle, 10);
    EXPECT_TRUE(em.has<MyComponent>(handle));

    cmp->remove = false;
    EXPECT_FALSE(em.remove<MyComponent>(handle));
    cmp->remove = true;
    EXPECT_TRUE(em.remove<MyComponent>(handle));

    EXPECT_FALSE(em.has<MyComponent>(handle));

}