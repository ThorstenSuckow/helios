module;


export module helios.math.camera;

import helios.math.core;

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
     *                    clip = P * V * W * M.
     *
     * @param eye
     * @param center
     * @param up
     * @return
     */
    mat4 lookAt(const vec3& eye, const vec3& center, const vec3& up) noexcept {

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