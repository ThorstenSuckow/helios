#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <array>


import helios.math;

namespace math = helios::math;

struct test_data {
    math::mat4 model;
    glm::mat4 glm_model;
    std::array<float, 3> vector;
    float angle;
};

static test_data setup() {

    time_t seed = time(0);
    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

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

    float angle = 45;
    float x = 0.0f;
    float y = 1.0f;
    float z = 0.0f;

    return test_data{
        .model = model,
        .glm_model = glm_model,
        .vector = {x, y, z},
        .angle = angle
    };
}


TEST(TransformTest, rotateModel) {

    // init data
    test_data data = setup();
    float x = data.vector[0], y = data.vector[1], z = data.vector[2];
    float angle = data.angle;
    math::mat4 model = data.model;
    glm::mat4 glm_model = data.glm_model;

    // apply rotations
    auto axis = math::vec3(x, y, z);
    const math::mat4 R = math::rotate(model, math::radians(angle), axis);
    auto glm_axis = glm::normalize(glm::vec3(x, y, z));
    glm::mat4 glm_R = glm::rotate(glm_model, glm::radians(angle), glm_axis);
    const float* ptr = math::value_ptr(R);
    const float* glm_ptr = glm::value_ptr(glm_R);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}


TEST(TransformTest, translateModel) {

    // init data
    test_data data = setup();
    float x = data.vector[0], y = data.vector[1], z = data.vector[2];
    math::mat4 model = data.model;
    glm::mat4 glm_model = data.glm_model;

    // apply translation
    auto axis = math::vec3(x, y, z);
    const math::mat4 T = math::translate(model, axis);

    auto glm_axis = glm::vec3(x, y, z);
    glm::mat4 glm_T = glm::translate(glm_model, glm_axis);

    const float* ptr = math::value_ptr(T);
    const float* glm_ptr = glm::value_ptr(glm_T);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}

TEST(TransformTest, scaleModel_vec3) {

    // init data
    test_data data = setup();
    float x = data.vector[0], y = data.vector[1], z = data.vector[2];
    math::mat4 model = data.model;
    glm::mat4 glm_model = data.glm_model;

    // apply translation
    auto axis = math::vec3(x, y, z);
    const math::mat4 S = math::scale(model, axis);

    auto glm_axis = glm::vec3(x, y, z);
    glm::mat4 glm_S = glm::scale(glm_model, glm_axis);

    const float* ptr = math::value_ptr(S);
    const float* glm_ptr = glm::value_ptr(glm_S);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}

TEST(TransformTest, scaleModel_float) {

    // init data
    test_data data = setup();
    float x = data.vector[0];
    math::mat4 model = data.model;
    glm::mat4 glm_model = data.glm_model;

    // apply translation
    const float scale_by = x;
    const math::mat4 S = math::scale(model, scale_by);

    auto glm_axis = glm::vec3(scale_by, scale_by, scale_by);
    glm::mat4 glm_S = glm::scale(glm_model, glm_axis);

    const float* ptr = math::value_ptr(S);
    const float* glm_ptr = glm::value_ptr(glm_S);

    for (int i = 0; i < 16; i++) {
        EXPECT_FLOAT_EQ(ptr[i], glm_ptr[i]);
    }
}


