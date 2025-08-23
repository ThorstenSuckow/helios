#include <benchmark/benchmark.h>
#include <glm/vec3.hpp>

import helios.math;


static void BM_vec3(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(helios::math::vec3());
    }
}
BENCHMARK(BM_vec3)->UseRealTime();

static void BM_glm_vec3(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(glm::vec3());
    }
}
BENCHMARK(BM_glm_vec3)->UseRealTime();


BENCHMARK_MAIN();