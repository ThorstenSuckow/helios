/**
 * @file PerspectiveCameraContext.ixx
 * @brief Camera projection parameters used by scene culling and rendering.
 */
module;

#include <cassert>

export module helios.scene.types.PerspectiveCameraContext;


import helios.math;

using namespace helios::math;

export namespace helios::scene::types {

    /**
     * @brief Immutable projection context for perspective-camera dependent operations.
     */
    struct PerspectiveCameraContext {

        /**
         * @brief The aspect ratio of the camera (width/height).
         *
         * Defaults to 1.0.
         */
        const float aspectRatio_ = 1.0f;

        /**
         * @brief The near clipping plane distance.
         *
         * Defaults to 0.1.
         */
        const float zNear_ = 0.1f;

        /**
         * @brief The far clipping plane distance.
         *
         * Defaults to 1000.0.
         */
        const float zFar_ = 1000.0f;

        /**
         * @brief The vertical field of view in radians.
         *
         * Defaults to 90 degrees (converted to radians).
         */
        const float fovY_ = radians(90);


    };

}