module;

#include <cmath>

module helios.math.transform;

import :camera;

import helios.math.types;
import helios.math.utils;

namespace helios::math {

    mat4f lookAt(const vec3f& eye,const vec3f& center, const vec3f& up) noexcept {
        const auto z = (eye - center).normalize();
        const auto x = cross(up, z).normalize();
        const auto y = cross(z, x).normalize();

        return mat4{
            x[0], y[0], z[0], 0.0f,
            x[1], y[1], z[1], 0.0f,
            x[2], y[2], z[2], 0.0f,
            -dot(x, eye), -dot(y, eye), -dot(z, eye), 1.0f,
        };
    }

    mat4f perspective(const float fovY, const float aspect, const float zNear, const float zFar) noexcept {

        float f = 1 / std::tan(fovY/2);

        return mat4{
            f/aspect, 0.0f, 0.0f, 0.0f,
            0.0f, f, 0.0f, 0.0f,
            0.0f, 0.0f , -(zFar + zNear)/(zFar - zNear), -1.0f,
            0.0f, 0.0f, -2 * (zFar * zNear) / (zFar - zNear), 0.0f
        };

    }

}

