module;

export module helios.math.transform:model;

import helios.math.types;

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
    mat4f rotate(const mat4f& model, float radians, const vec3f& axis) noexcept;

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