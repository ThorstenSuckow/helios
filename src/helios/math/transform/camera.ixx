module;


export module helios.math.transform:camera;

import helios.math.types;

export namespace helios::math {

    /**
     * Returns the 4x4 lookAt-matrix for transforming world coordinates
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
    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) noexcept;

}