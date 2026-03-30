/**
 * @file GameObject.benchmark.cpp
 * @brief Benchmarks for iterating over GameObjects and querying components.
 *
 * Measures the cost of component lookups in the legacy GameObject-based design.
 */
#include <benchmark/benchmark.h>

#include <memory>
#include <vector>

import helios.engine.game.GameObject;
import helios.engine.game.Component;
import helios.engine.game.components.physics.Move2DComponent;
import helios.engine.game.components.physics.TransformComponent;
import helios.engine.game.components.physics.LevelBoundsBehaviorComponent;
import helios.engine.game.components.physics.ScaleComponent;
import helios.engine.game.components.physics.AabbColliderComponent;
import helios.core.units;


using namespace helios::engine::game;
using namespace helios::engine::game::components::physics;


/**
 * Helper function: populate a vector with N GameObjects.
 * - every object gets a TransformComponent
 * - every 2nd object also gets a Move2DComponent
 * - every 3rd object also gets a LevelBoundsBehaviorComponent
 * - every 5th object also gets a ScaleComponent
 */
static std::vector<std::unique_ptr<GameObject>> createGameObjects(std::size_t count) {
    std::vector<std::unique_ptr<GameObject>> objects;
    objects.reserve(count);

    for (std::size_t i = 0; i < count; ++i) {
        auto obj = std::make_unique<GameObject>();

        obj->add<TransformComponent>();

        if (i % 2 == 0) {
            obj->add<Move2DComponent>();
        }
        if (i % 3 == 0) {
            obj->add<LevelBoundsBehaviorComponent>();
        }
        if (i % 5 == 0) {
            obj->add<ScaleComponent>(1.0f, 1.0f, 1.0f, helios::core::units::Unit::Meter);
        }

        objects.push_back(std::move(obj));
    }

    return objects;
}


// measures has<T>() (returns bool, uses dynamic_cast)
static void BM_HasComponent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    auto objects = createGameObjects(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto& obj : objects) {
            if (obj->has<Move2DComponent>()) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_HasComponent)
    ->Arg(100)
    ->Arg(200)
    ->Arg(500)
    ->Arg(1000)
    ->Arg(2000)
    ->Arg(5000)
    ->Arg(10000)
    ->Arg(20000)
    ->Arg(50000)
    ->Unit(benchmark::kMicrosecond);


// measures get<T>() (returns pointer, uses dynamic_cast)
static void BM_GetComponent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    auto objects = createGameObjects(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto& obj : objects) {
            if (obj->get<Move2DComponent>() != nullptr) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_GetComponent)
    ->Arg(100)
    ->Arg(200)
    ->Arg(500)
    ->Arg(1000)
    ->Arg(2000)
    ->Arg(5000)
    ->Arg(10000)
    ->Arg(20000)
    ->Arg(50000)
    ->Unit(benchmark::kMicrosecond);


// BM_GetMultipleComponents - measures querying several component types per
// object (similar to what LevelBoundsBehaviorSystem does each frame).
static void BM_GetMultipleComponents(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    auto objects = createGameObjects(count);

    for (auto _ : state) {
        std::size_t matched = 0;
        for (const auto& obj : objects) {
            auto* m2d = obj->get<Move2DComponent>();
            if (!m2d) continue;

            auto* tc  = obj->get<TransformComponent>();
            auto* bbc = obj->get<LevelBoundsBehaviorComponent>();
            auto* sc  = obj->get<ScaleComponent>();

            if (tc && bbc && sc) {
                ++matched;
            }
        }
        benchmark::DoNotOptimize(matched);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_GetMultipleComponents)
    ->Arg(100)
    ->Arg(200)
    ->Arg(500)
    ->Arg(1000)
    ->Arg(2000)
    ->Arg(5000)
    ->Arg(10000)
    ->Arg(20000)
    ->Arg(50000)
    ->Unit(benchmark::kMicrosecond);


// BM_HasComponent_Absent - worst-case: component is never present
static void BM_HasComponent_Absent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    auto objects = createGameObjects(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (const auto& obj : objects) {
            if (obj->has<AabbColliderComponent>()) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

// use various sizes for scaling comparisons
BENCHMARK(BM_HasComponent_Absent)
    ->Arg(100)
    ->Arg(200)
    ->Arg(500)
    ->Arg(1000)
    ->Arg(2000)
    ->Arg(5000)
    ->Arg(10000)
    ->Arg(20000)
    ->Arg(50000)
    ->Unit(benchmark::kMicrosecond);


BENCHMARK_MAIN();



