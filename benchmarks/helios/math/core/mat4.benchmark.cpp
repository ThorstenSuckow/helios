#include <benchmark/benchmark.h>
#include <glm/mat4x4.hpp>

import helios.math;

namespace math = helios::math;

static void BM_mat4Constructor(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(
            math::mat4{
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11,
            12, 13, 14, 15
        });
    }
}
BENCHMARK(BM_mat4Constructor)->UseRealTime();

static void BM_glm_mat4Constructor(benchmark::State& state) {
    for (auto _ : state) {
    benchmark::DoNotOptimize(
        glm::mat4(
            0, 1, 2, 3,
            4, 5, 6, 7,
            8, 9, 10, 11,
            12, 13, 14, 15
        ));
    }
}
BENCHMARK(BM_glm_mat4Constructor)->UseRealTime();


static void BM_mat4Multiply(benchmark::State& state) {

    math::mat4 M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    math::mat4 A{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    auto multiply_matrices = [&]( ) {
        math::mat4 B = A * M;
    };

    for (auto _ : state) {
        multiply_matrices();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_mat4Multiply)->UseRealTime();


static void BM_glm_mat4Multiply(benchmark::State& state) {

    glm::mat4 M{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    glm::mat4 A{
        0, 1, 2, 3,
        4, 5, 6, 7,
        8, 9, 10, 11,
        12, 13, 14, 15
    };

    auto multiply_matrices = [&]( ) {
        glm::mat4 B = A * M;
    };

    for (auto _ : state) {
        multiply_matrices();
        benchmark::ClobberMemory();
    }
}
BENCHMARK(BM_glm_mat4Multiply)->UseRealTime();


BENCHMARK_MAIN();

