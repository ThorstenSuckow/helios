/**
 * @file EntityComponent.benchmark.cpp
 * @brief ECS benchmarks comparable to the Milestone 3 GameObject benchmarks.
 *
 * Measures the cost of SparseSet-based component lookups (has<T>(), get<T>())
 * over entities with varying component configurations.
 *
 * These benchmarks mirror the structure of GameObject.benchmark.cpp (v0.3.0-milestone3)
 * to allow a direct comparison between the dynamic_cast-based approach and
 * the ECS / SparseSet approach (v0.5.0.-milestone5).
 */
#include <benchmark/benchmark.h>

#include <cstddef>
#include <vector>

import helios.engine.ecs.EntityManager;
import helios.engine.ecs.EntityRegistry;
import helios.engine.ecs.EntityHandle;
import helios.engine.ecs.View;
import helios.engine.ecs.GameObject;

import helios.engine.modules.physics.motion.components.Move2DComponent;
import helios.engine.modules.spatial.transform.components.TranslationStateComponent;
import helios.engine.mechanics.bounds.components.LevelBoundsBehaviorComponent;
import helios.engine.modules.spatial.transform.components.ScaleStateComponent;
import helios.engine.modules.physics.collision.components.AabbColliderComponent;
import helios.core.units.Unit;


using namespace helios::engine::ecs;
using namespace helios::engine::modules::physics::motion::components;
using namespace helios::engine::modules::spatial::transform::components;
using namespace helios::engine::mechanics::bounds::components;
using namespace helios::engine::modules::physics::collision::components;

/**
 * @brief Holds EntityRegistry, EntityManager, and the created gameObjects.
 *
 * Mirrors createGameObjects() from the Milestone 3 benchmark:
 *   - Every entity gets a TranslationStateComponent.
 *   - Every 2nd entity also gets a Move2DComponent.
 *   - Every 3rd entity also gets a LevelBoundsBehaviorComponent.
 *   - Every 5th entity also gets a ScaleStateComponent.
 */
struct EcsFixture {
    EntityRegistry registry;
    EntityManager em;
    std::vector<GameObject> gameObjects;

    explicit EcsFixture(std::size_t count)
        : em(registry, count) {

        gameObjects.reserve(count);

        for (std::size_t i = 0; i < count; ++i) {
            GameObject gameObject{ em.create(), &em};

            gameObject.add<TranslationStateComponent>();

            if (i % 2 == 0) {
                gameObject.add<Move2DComponent>();
            }
            if (i % 3 == 0) {
                gameObject.add<LevelBoundsBehaviorComponent>();
            }
            if (i % 5 == 0) {
                gameObject.add<ScaleStateComponent>(1.0f, 1.0f, 1.0f, helios::core::units::Unit::Meter);
            }

            gameObjects.push_back(gameObject);
        }
    }
};


// 1) BM_ECS_HasComponent
//    equivalent to BM_HasComponent - per-handle has<T>() via SparseSet::contains
static void BM_ECS_HasComponent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    EcsFixture fixture(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (auto& go : fixture.gameObjects) {
            if (go.has<Move2DComponent>()) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_ECS_HasComponent)
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


// 2) BM_ECS_GetComponent
//    equivalent to BM_GetComponent - per-handle get<T>() via SparseSet
static void BM_ECS_GetComponent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    EcsFixture fixture(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (auto& go : fixture.gameObjects) {
            if (go.get<Move2DComponent>() != nullptr) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_ECS_GetComponent)
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


// 3) BM_ECS_GetMultipleComponents
//    equivalent to BM_GetMultipleComponents - querying several types per entity
static void BM_ECS_GetMultipleComponents(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    EcsFixture fixture(count);

    for (auto _ : state) {
        std::size_t matched = 0;
        for (auto& go : fixture.gameObjects) {
            auto* m2d = go.get<Move2DComponent>();
            if (!m2d) continue;

            auto* tsc = go.get<TranslationStateComponent>();
            auto* lbb = go.get<LevelBoundsBehaviorComponent>();
            auto* sc  = go.get<ScaleStateComponent>();

            if (tsc && lbb && sc) {
                ++matched;
            }
        }
        benchmark::DoNotOptimize(matched);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_ECS_GetMultipleComponents)
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


// 4) BM_ECS_HasComponent_Absent
//    equivalent to BM_HasComponent_Absent - component never present
static void BM_ECS_HasComponent_Absent(benchmark::State& state) {
    const auto count = static_cast<std::size_t>(state.range(0));
    EcsFixture fixture(count);

    for (auto _ : state) {
        std::size_t found = 0;
        for (auto& go : fixture.gameObjects) {
            if (go.has<AabbColliderComponent>()) {
                ++found;
            }
        }
        benchmark::DoNotOptimize(found);
    }

    state.SetItemsProcessed(
        static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(count)
    );
}

BENCHMARK(BM_ECS_HasComponent_Absent)
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

