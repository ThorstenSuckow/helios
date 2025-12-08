/**
 * @file Camera.ixx
 * @brief Defines the Camera class providing view and projection matrices.
 */
module;

export module helios.scene.Camera;

import helios.math.utils;
import helios.math.transform;
import helios.math.types;
import helios.scene.SceneNode;

export namespace helios::scene {

    /**
     * @brief Represents a camera for perspective projection.
     *
     * The camera provides the view and projection matrices, whereas the view matrix
     * is computed as the inverse of the camera's world transform.
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
        mutable float aspectRatio_ = 1.0f;

        /**
         * @brief The near clipping plane distance.
         *
         * Defaults to 0.1.
         */
        mutable float zNear_ = 0.1f;

        /**
         * @brief The far clipping plane distance.
         *
         * Defaults to 1000.0.
         */
        mutable float zFar_ = 1000.0f;

        /**
         * @brief The vertical field of view in radians.
         *
         * Defaults to 90 degrees (converted to radians).
         */
        mutable float fovY_ = helios::math::radians(90);

        /**
         * @brief Flag indicating whether the matrices need to be updated.
         */
        mutable bool needsUpdate_ = true;



        /**
         * @brief Updates the view and projection matrices if needed.
         *
         * This function recalculates the view and projection matrices
         * based on the current camera parameters.
         */
        void update() const {

            if (!needsUpdate_) {
                return;
            }

            projectionMatrix_ = helios::math::perspective(fovY_, aspectRatio_, zNear_, zFar_);

            needsUpdate_ = false;
        }

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
         * @brief Sets the view matrix for this camera.
         *
         * This method is typically called by `CameraSceneNode::worldTransform()` to update
         * the view matrix based on the camera node's position and orientation in the scene graph.
         *
         * @param viewMatrix The new view matrix to assign.
         *
         * @return A const reference to this camera instance.
         *
         * @see helios::scene::CameraSceneNode::worldTransform()
         */
        const Camera& setViewMatrix(const helios::math::mat4f& viewMatrix) const noexcept {
            viewMatrix_ = viewMatrix;
            return *this;
        }

        /**
         * @brief Sets the aspect ratio used by the camera.
         *
         * @param aspectRatio The new aspect ratio (width/height).
         * @return A reference to this camera instance.
         */
        const Camera& setAspectRatio(float aspectRatio) const noexcept;

        /**
         * @brief Sets the perspective projection parameters.
         *
         * @param fovY The vertical field of view in radians.
         * @param aspectRatio The aspect ratio (width/height).
         * @param zNear The near clipping plane distance.
         * @param zFar The far clipping plane distance.
         * @return A reference to this camera instance.
         */
        const Camera& setPerspective(float fovY, float aspectRatio, float zNear, float zFar) const noexcept;


        /**
         * @brief Sets the near clipping plane distance.
         *
         * @param zNear The new near clipping plane distance.
         * @return A reference to this camera instance.
         */
        const Camera& setZNear(const float zNear) const noexcept {
            zNear_ = zNear;
            needsUpdate_ = true;
            return *this;
        }

        /**
         * @brief Sets the far clipping plane distance.
         *
         * @param zFar The new far clipping plane distance.
         * @return A reference to this camera instance.
         */
        const Camera& setZFar(const float zFar) const noexcept {
            zFar_ = zFar;
            needsUpdate_ = true;
            return *this;
        }

        /**
         * @brief Sets the vertical field of view.
         *
         * @param fovY The new vertical field of view in radians.
         * @return A reference to this camera instance.
         */
        const Camera& setFovy(const float fovY) const noexcept {
            fovY_ = fovY;
            needsUpdate_ = true;
            return *this;
        }


        /**
         * @brief Gets the current vertical field of view.
         *
         * @return The vertical field of view in radians.
         */
        [[nodiscard]] float fovY() const noexcept {
            return fovY_;
        }

        /**
         * @brief Gets the current aspect ratio.
         *
         * @return The aspect ratio (width/height).
         */
        [[nodiscard]] float aspectRatio() const noexcept {
            return aspectRatio_;
        }

        /**
         * @brief Gets the current near clipping plane distance.
         *
         * @return The near clipping plane distance.
         */
        [[nodiscard]] float zNear() const noexcept {
            return zNear_;
        }

        /**
         * @brief Gets the current far clipping plane distance.
         *
         * @return The far clipping plane distance.
         */
        [[nodiscard]] float zFar() const noexcept {
            return zFar_;
        }

    };

}