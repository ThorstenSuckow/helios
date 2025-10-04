module;

#include <cmath>

module helios.math.transform;

import :model;

import helios.math.types;

namespace helios::math::transform {

    constexpr mat4 make_rodrigues_rotation_matrix(
        const float cos_theta,
        const float sin_theta,
        const vec3f& normalized_axis
    ) noexcept {

        const float t = 1.0f - cos_theta;

        const float x = normalized_axis[0],
            y = normalized_axis[1],
            z = normalized_axis[2];


        return mat4{
            cos_theta + x * x * t,
            x * y *t + z * sin_theta,
            x * z * t - y * sin_theta,
            0,

            x * y * t - z * sin_theta,
            cos_theta + y * y * t,
            y * z * t + x * sin_theta,
            0,

            x * z * t + y * sin_theta,
            y * z * t - x * sin_theta,
            cos_theta + z * z * t,
            0,

            0, 0, 0, 1
        };
    };

};


namespace helios::math {


    mat4 rotate(const mat4& model, const float radians, const vec3f& axis) noexcept {

        return model * transform::make_rodrigues_rotation_matrix(
            std::cos(radians),
            std::sin(radians),
        axis.normalize()
        );
    };


};





