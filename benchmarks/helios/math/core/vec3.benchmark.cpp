#include <benchmark/benchmark.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>

import helios.math;

glm::vec3 glm_vec_a{1, 2, 3};
glm::vec3 glm_vec_b{4, 5, 6};

helios::math::vec3 vec_a{1, 2, 3};
helios::math::vec3 vec_b{4, 5, 6};


/**
 * Constructor
 */
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


/**
 * Cross
 */
static void BM_vec3Cross(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(helios::math::cross(vec_a, vec_b));
    }
}
BENCHMARK(BM_vec3Cross)->UseRealTime();

static void BM_glm_vec3Cross(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(glm::cross(glm_vec_a, glm_vec_b));
    }
}
BENCHMARK(BM_glm_vec3Cross)->UseRealTime();


/**
 * Dot
 */
static void BM_vec3Dot(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(helios::math::dot(vec_a, vec_b));
    }
}
BENCHMARK(BM_vec3Dot)->UseRealTime();

static void BM_glm_vec3Dot(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(glm::dot(glm_vec_a, glm_vec_b));
    }
}
BENCHMARK(BM_glm_vec3Dot)->UseRealTime();

BENCHMARK_MAIN();