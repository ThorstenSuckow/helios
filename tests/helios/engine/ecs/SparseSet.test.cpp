#include <gtest/gtest.h>
#include <helios/helios_config.h>

import helios.engine.ecs;
import helios.engine.core.data;

using namespace helios::engine::ecs;
using namespace helios::engine::core::data;

class Entity {

public:
    int value = 0;

    bool operator==(const Entity& other) const {
        return value == other.value;
    }
};


TEST(SparseSetTest, emplace) {

    SparseSet<Entity> storage;

    auto* ent = storage.emplace(EntityId{1}, Entity{});

    EXPECT_NE(ent, nullptr);

    EXPECT_EQ(storage.emplace(EntityId{1}, Entity{}), nullptr);


}

TEST(SparseSetTest, get) {

    SparseSet<Entity> storage;

    auto* ent = storage.emplace(EntityId{1}, Entity{10});

    auto* entGet = storage.get(EntityId{1});

    EXPECT_EQ(ent->value, entGet->value);
}


TEST(SparseSetTest, remove) {

    const std::function<bool(Entity& entity)> onRemoveCallbackFalse = [](Entity& entity) ->bool { return false;};
    const std::function<bool(Entity& entity)> onRemoveCallbackTrue = [](Entity& entity) ->bool { return true;};

    SparseSet<Entity> storage;

    auto* ent1 = storage.emplace(EntityId{1}, Entity{10});
    auto* ent2 = storage.emplace(EntityId{2}, Entity{20});
    auto* ent3 = storage.emplace(EntityId{3}, 30);
    auto* ent4 = storage.emplace(EntityId{4}, Entity{40});


    // no callback
    EXPECT_TRUE(storage.remove(EntityId{1}));
    EXPECT_EQ(storage.get(EntityId{1}), nullptr);
    EXPECT_EQ(storage.get(EntityId{2})->value, 20);
    EXPECT_EQ(storage.get(EntityId{3})->value, 30);
    EXPECT_EQ(storage.get(EntityId{4})->value, 40);

    // false callback
    storage.setOnRemoveCallback(onRemoveCallbackFalse);
    EXPECT_FALSE(storage.remove(EntityId{3}));
    EXPECT_EQ(storage.get(EntityId{1}), nullptr);
    EXPECT_EQ(storage.get(EntityId{2})->value, 20);
    EXPECT_EQ(storage.get(EntityId{3})->value, 30);
    EXPECT_EQ(storage.get(EntityId{4})->value, 40);

    // true callback
    storage.setOnRemoveCallback(onRemoveCallbackTrue);
    EXPECT_TRUE(storage.remove(EntityId{3}));
    EXPECT_EQ(storage.get(EntityId{1}), nullptr);
    EXPECT_EQ(storage.get(EntityId{2})->value, 20);
    EXPECT_EQ(storage.get(EntityId{3}), nullptr);
    EXPECT_EQ(storage.get(EntityId{4})->value, 40);

}