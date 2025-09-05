#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>
#include <ctime>


import helios.math;

namespace math = helios::math;

struct test_data {
    math::vec3 up;
    math::vec3 eye;
    math::vec3 center;

    glm::vec3 glm_up;
    glm::vec3 glm_eye;
    glm::vec3 glm_center;
};

static test_data setup() {

    time_t seed = time(0);
    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    float x1 = static_cast<float>(std::rand());
    float y1 = static_cast<float>(std::rand());
    float z1 = static_cast<float>(std::rand());
    float x2 = static_cast<float>(std::rand());
    float y2 = static_cast<float>(std::rand());
    float z2 = static_cast<float>(std::rand());
    float x3 = static_cast<float>(std::rand());
    float y3 = static_cast<float>(std::rand());
    float z3 = static_cast<float>(std::rand());

    return test_data{
        .up = math::vec3{x1, y1, z1},
        .eye = math::vec3{x2, y2, z2},
        .center = math::vec3{x3, y3, z3},

        .glm_up = glm::vec3{x1, y1, z1},
        .glm_eye = glm::vec3{x2, y2, z2},
        .glm_center = glm::vec3{x3, y3, z3},
    };
}


TEST(CameraTest, lookAt) {

    // init data
    test_data data = setup();

    math::vec3 up = data.up;
    math::vec3 eye = data.eye;
    math::vec3 center = data.center;

    glm::vec3 glm_up = data.glm_up;
    glm::vec3 glm_eye = data.glm_eye;
    glm::vec3 glm_center = data.glm_center;

    // apply rotations
    const math::mat4 lookAt = math::lookAt(eye, center, up);

    const glm::mat4 glm_lookAt = glm::lookAt(glm_eye, glm_center, glm_up);

    const float* ptr = math::value_ptr(lookAt);
    const float* glm_ptr = glm::value_ptr(glm_lookAt);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}


