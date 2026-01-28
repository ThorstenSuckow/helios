/**
 * @file model.ixx
 * @brief Model transform helper functions.
 */
module;

#include <cmath>

export module helios.math.transform:model;

import helios.math.types;

namespace helios::math::transform {

    constexpr mat4f make_rodrigues_rotation_matrix(
        const float cos_theta,
        const float sin_theta,
        const vec3f& normalized_axis
    ) noexcept {

        const float t = 1.0f - cos_theta;

        const float x = normalized_axis[0],
            y = normalized_axis[1],
            z = normalized_axis[2];


        return mat4f{
            cos_theta + x * x * t,
            x * y * t + z * sin_theta,
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

export namespace helios::math {

    /**
     * @brief Creates an affine rotation matrix R' = M * R by "baking" the rotation part into the model matrix.
     * Any operation R' * v will make sure that v is rotated around the local origin, then transformed into
     * world space.
     *
     * @param model
     * @param radians
     * @param axis
     * @return
     */
    inline mat4f rotate(const mat4f& model, float radians, const vec3f& axis) noexcept {


        return model * transform::make_rodrigues_rotation_matrix(
            std::cos(radians),
            std::sin(radians),
        axis.normalize()
        );
    }

    /**
     * @brief Creates an affine transformation matrix M' = M * T by "baking" the translation part into the model
     * matrix.
     * Any operation M' * v will make sure that v is translated in local space, then transformed into
     * world space.
     *
     * @param model
     * @param v
     * @return
     */
     constexpr mat4f translate(const mat4f& model, const vec3f& v) noexcept {
         mat4 t = model;
         t(0, 3) += t(0,0) * v[0] + t(0, 1) * v[1] + t(0, 2) * v[2];
         t(1, 3) += t(1,0) * v[0] + t(1, 1) * v[1] + t(1, 2) * v[2];
         t(2, 3) += t(2,0) * v[0] + t(2, 1) * v[1] + t(2, 2) * v[2];
         return t;
     };

     /**
      * @brief Creates an affine transformation matrix S' = M * S by ""baking" the scaling part into
      * the model matrix.
      * Any operation S' * v will make sure that v is scaled in local space, then transformed into world
      * space.
      *
      * @param model
      * @param v
      * @return
      */
    constexpr mat4f scale(const mat4f& model, const vec3f& v) noexcept {
        mat4f t = model;

        t(0, 0) *= v[0]; t(0, 1) *= v[1]; t(0, 2) *= v[2];
        t(1, 0) *= v[0]; t(1, 1) *= v[1]; t(1, 2) *= v[2];
        t(2, 0) *= v[0]; t(2, 1) *= v[1]; t(2, 2) *= v[2];

        return t;
    }

    /**
     * @brief Creates an affine transformation matrix S' = M * S by "baking" the scaling part into
     * the model matrix.
     * Any operation S' * v will make sure that v is scaled in local space, then transformed into world
     * space.
     *
     * @param model
     * @param scale_by
     * @return
     */
    constexpr mat4f scale(const mat4f& model, const float scale_by) noexcept {
         mat4f t = model;

        t(0, 0) *= scale_by; t(0, 1) *= scale_by; t(0, 2) *= scale_by;
        t(1, 0) *= scale_by; t(1, 1) *= scale_by; t(1, 2) *= scale_by;
        t(2, 0) *= scale_by; t(2, 1) *= scale_by; t(2, 2) *= scale_by;

         return t;
     }

};