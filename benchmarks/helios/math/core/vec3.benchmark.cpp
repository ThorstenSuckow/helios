#include <benchmark/benchmark.h>
#include <glm/vec3.hpp>

import helios.math;


static void BM_vec3Constructor(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(helios::math::vec3());
    }
}
BENCHMARK(BM_vec3Constructor)->UseRealTime();

static void BM_glm_vec3Constructor(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(glm::vec3());
    }
}
BENCHMARK(BM_glm_vec3Constructor)->UseRealTime();


BENCHMARK_MAIN();