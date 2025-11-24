/**
 * @file Camera.ixx
 * @brief Defines the Camera class providing view and projection matrices.
 */
module;

#include <memory>

export module helios.scene.Camera;

import helios.math.types;
import helios.scene.SceneNode;

export namespace helios::scene {

    /**
     * @brief Represents a camera for perspective or orthographic projection.
     *
     * The camera provides the view and projection matrices, whereas the view matrix
     * is computed as the inverse of the camera's world transform.
     *
     * @todo Implement automatic recalculation of projection and view matrices when
     *       local transforms change (dirty flag pattern).
     */
    class Camera {

    protected:
        /**
         * @brief The projection matrix of this camera.
         */
        mutable helios::math::mat4f projectionMatrix_;

        /**
         * @brief The view matrix of this camera.
         */
        mutable helios::math::mat4f viewMatrix_;

        /**
         * @brief The aspect ratio of the camera (width/height).
         *
         * Defaults to 1.0.
         */
        float aspectRatio_ = 1.0f;

    public:
        /**
         * @brief Constructs a camera with identity projection and view matrices.
         */
        Camera() noexcept;

        /**
         * @brief Gets the current projection matrix.
         *
         * Typically set to a perspective or orthographic projection.
         *
         * @return A const reference to this camera's projection matrix.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() const noexcept;

        /**
         * @brief Gets the current view matrix.
         *
         * The view matrix represents the inverse of the camera's world transform.
         *
         * @return A const reference to this camera's view matrix.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() const noexcept;

        /**
         * @brief Sets the aspect ratio used by the camera.
         *
         * @param aspectRatio The new aspect ratio (width/height).
         */
        void setAspectRatio(float aspectRatio) noexcept;

    };

}
