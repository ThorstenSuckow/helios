module;

export module helios.math.transform;

import helios.math.core;

export namespace helios::math {

    /**
     * Creates an affine rotation matrix R' = M * R by "baking" the rotation part into the model matrix.
     * Any operation R' * v will make sure that v is rotated around the local origin, then transformed into
     * world space.
     *
     * @param model
     * @param radians
     * @param axis
     * @return
     */
    mat4 rotate(const mat4& model, float radians, const vec3& axis) noexcept;

    /**
     * Creates an affine transformation matrix M' = M * T by "baking" the translation part into the model
     * matrix.
     * Any operation M' * v will make sure that v is translated in local space, then transformed into
     * world space.
     *
     * @param model
     * @param v
     * @return
     */
     constexpr mat4 translate(const mat4& model, const vec3& v) noexcept {
         mat4 t = model;
         t(0, 3) += t(0,0) * v[0] + t(0, 1) * v[1] + t(0, 2) * v[2];
         t(1, 3) += t(1,0) * v[0] + t(1, 1) * v[1] + t(1, 2) * v[2];
         t(2, 3) += t(2,0) * v[0] + t(2, 1) * v[1] + t(2, 2) * v[2];
         return t;
     };

};