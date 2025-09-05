#include <benchmark/benchmark.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

import helios.math;

glm::vec3 glm_vec_eye{1, 2, 3};
glm::vec3 glm_vec_up{4, 5, 6};
glm::vec3 glm_vec_center{7, 8, 9};


helios::math::vec3 vec_eye{1, 2, 3};
helios::math::vec3 vec_up{4, 5, 6};
helios::math::vec3 vec_center{7, 8, 9};

/**
 * lookAt
 */
static void BM_lookAt(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(helios::math::lookAt(vec_eye, vec_center, vec_up));
    }
}
BENCHMARK(BM_lookAt)->UseRealTime();

static void BM_glm_lookAt(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(glm::lookAt(glm_vec_eye, glm_vec_center, glm_vec_up));
    }
}
BENCHMARK(BM_glm_lookAt)->UseRealTime();


BENCHMARK_MAIN();