/**
 * @file camera.ixx
 * @brief Camera transform helpers.
 */
module;


export module helios.math.transform:camera;

import helios.math.types;


export namespace helios::math {

    /**
     * @brief Computes the 4x4 perspective projection matrix based on the specified
     * field of view, aspect ratio, and near and far clipping planes.
     *
     * This method returns a matrix suitable for projecting 3D points onto a
     * 2D screen from the perspective of a camera. The perspective transformation
     * maps 3D points inside the specified viewing frustum to normalized device
     * coordinates, which are then clipped and transformed to screen space.
     *
     * @param fovY Field of view in the Y direction, specified in radians.
     * @param aspect The aspect ratio of the view, defined as width divided by height.
     * @param zNear The distance to the near clipping plane. Must be greater than zero.
     * @param zFar The distance to the far clipping plane. Must be greater than zNear.
     *
     * @return A 4x4 matrix representing the perspective projection transformation.
     *
     * @see https://thorsten.suckow-homberg.de/docs/articles/computer-graphics/from-camera-to-clip-space-derivation-of-the-projection-matrices
     */
    mat4f perspective(float fovY, float aspect, float zNear, float zFar) noexcept;

    /**
     * @brief Returns the 4x4 lookAt-matrix for transforming world coordinates
     * to camera space.
     * The method creates the camera coordinate system based on the
     * parameters eye, i.e. the vantage point in world space, the camera's
     * up direction and center, representing the point of interest.
     * Note that the returned matrix is essentially a composition of
     * a change of coordinates matrix P(C <- W) and a translation
     * to the origin, i.e. the camera is sitting at the origin (0, 0, 0)
     * looking down the negative z-axis, i.e. conceptually
     *
     *                      P(W<-C) * T(-eye)
     *
     * The 4x4 matrix can then be used for computing the perspective projection
     * for creating the clip space
     *
     *                    clip = P * V * W * M
     *
     * @param eye
     * @param center
     * @param up
     * @return
     */
    mat4f lookAt(const vec3f& eye, const vec3f& center, const vec3f& up) noexcept;

}