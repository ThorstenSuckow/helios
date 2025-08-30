#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../../.build/cmake_build_release/_deps/benchmark-src/src/arraysize.h"


import helios.math;

namespace math = helios::math;

TEST(TransformTest, rotateModel) {

    std::srand(time(0));

    auto model = math::mat4{1.0f};
    auto glm_model = glm::mat4{1.0f};
    float* v = math::value_ptr(model);
    float* glm_v = glm::value_ptr(glm_model);

    for (int i = 0; i < 15; i++) {
        int z = std::rand();
        v[i] = z;
        glm_v[i] = z;
    }
    v[3] = glm_v[3] = v[7] = glm_v[7] = v[11] = glm_v[11] = 0;
    v[15] = glm_v[15] = 1;


    constexpr float angle = 45;
    constexpr float x = 0.0f;
    constexpr float y = 1.0f;
    constexpr float z = 0.0f;

    constexpr auto axis = math::vec3(x, y, z);
    const math::mat4 R = math::rotate(model, math::radians(angle), axis);

    auto glm_axis = glm::normalize(glm::vec3(x, y, z));
    glm::mat4 glm_R = glm::rotate(glm_model, glm::radians(angle), glm_axis);

    const float* ptr = math::value_ptr(R);
    const float* glm_ptr = glm::value_ptr(glm_R);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}


