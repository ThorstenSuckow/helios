module helios.math.transform;

import :camera;

import helios.math.types;

namespace helios::math {

    mat4 lookAt(const vec3& eye,const vec3& center, const vec3& up) noexcept {
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

}

