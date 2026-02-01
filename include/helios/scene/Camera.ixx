/**
 * @file Camera.ixx
 * @brief Defines the Camera class providing view and projection matrices.
 */
module;

#include <cassert>

export module helios.scene.Camera;

import helios.math.utils;
import helios.math.transform;
import helios.math.types;
import helios.scene.SceneNode;

export namespace helios::scene {

    /**
     * @brief Represents a camera for perspective or orthographic projection.
     *
     * The camera provides the view and projection matrices, whereas the view matrix
     * is computed as the inverse of the camera's world transform.
     *
     * The camera supports two projection modes:
     * - **Perspective projection:** Simulates how the human eye perceives depth, where
     *   distant objects appear smaller. Use `setPerspective()` to configure.
     * - **Orthographic projection:** Maintains parallel lines and uniform scaling regardless
     *   of distance, useful for 2D rendering, UI elements, or CAD-style views. Use `setOrtho()`
     *   to configure.
     *
     * By default, the camera uses perspective projection.
     *
     * Example usage:
     * ```cpp
     * helios::scene::Camera camera;
     *
     * // Perspective projection
     * camera.setPerspective(helios::math::radians(60.0f), 16.0f / 9.0f, 0.1f, 1000.0f);
     *
     * // Orthographic projection
     * camera.setOrtho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 100.0f);
     * ```
     */
    class Camera {

    protected:
        /**
         * @brief The projection matrix of this camera.
         */
        mutable helios::math::mat4f perspectiveMatrix_;

        /**
         * @brief The orthographic projection matrix of this camera.
         *
         * Used when the camera is in orthographic mode (`usePerspective_ == false`).
         */
        mutable helios::math::mat4f orthographicMatrix_;

        /**
         * @brief The view matrix of this camera.
         */
        helios::math::mat4f viewMatrix_;

        /**
         * @brief The aspect ratio of the camera (width/height).
         *
         * Defaults to 1.0.
         */
        float aspectRatio_ = 1.0f;

        /**
         * @brief The near clipping plane distance.
         *
         * Defaults to 0.1.
         */
        float zNear_ = 0.1f;

        /**
         * @brief The far clipping plane distance.
         *
         * Defaults to 1000.0.
         */
        float zFar_ = 1000.0f;

        /**
         * @brief The vertical field of view in radians.
         *
         * Defaults to 90 degrees (converted to radians).
         */
        float fovY_ = helios::math::radians(90);

        /**
         * @brief Flag indicating whether the matrices need to be updated.
         */
        mutable bool needsUpdate_ = true;

        /**
         * @brief Left boundary of the orthographic frustum.
         */
        float left_;

        /**
         * @brief Right boundary of the orthographic frustum.
         */
        float right_;

        /**
         * @brief Bottom boundary of the orthographic frustum.
         */
        float bottom_;

        /**
         * @brief Top boundary of the orthographic frustum.
         */
        float top_;

        /**
         * @brief Flag indicating the projection mode.
         *
         * When `true`, perspective projection is used. When `false`, orthographic
         * projection is used. Defaults to `true`.
         */
        bool usePerspective_ = true;

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

            if (usePerspective_) {
                perspectiveMatrix_ = helios::math::perspective(fovY_, aspectRatio_, zNear_, zFar_);
            } else {
                orthographicMatrix_ = helios::math::ortho(left_, right_, bottom_, top_, zNear_, zFar_);
            }

            needsUpdate_ = false;
        }

    public:
        /**
         * @brief Constructs a camera with identity projection and view matrices.
         */
        Camera() noexcept :
            perspectiveMatrix_(helios::math::mat4f::identity()),
            orthographicMatrix_(helios::math::mat4f::identity()),
            viewMatrix_(helios::math::mat4f::identity())
        {}

        /**
         * @brief Gets the current projection matrix.
         *
         * Typically set to a perspective or orthographic projection.
         *
         * @return A const reference to this camera's projection matrix.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() const noexcept {
            update();
            
            if (usePerspective_) {
                return perspectiveMatrix_;
            }
            return orthographicMatrix_;
        }


        /**
         * @brief Gets the current view matrix.
         *
         * The view matrix represents the inverse of the camera's world transform.
         *
         * @return A const reference to this camera's view matrix.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() const noexcept {
            return viewMatrix_;
        }

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
        Camera& setViewMatrix(const helios::math::mat4f& viewMatrix) noexcept {
            viewMatrix_ = viewMatrix;
            return *this;
        }

        /**
         * @brief Sets the aspect ratio used by the camera.
         *
         * @param aspectRatio The new aspect ratio (width/height).
         *
         * @return A reference to this camera instance.
         */
        Camera& setAspectRatio(float aspectRatio) noexcept {
            needsUpdate_ = true;
            aspectRatio_ = aspectRatio;
            return *this;
        }

        /**
         * @brief Handles viewport resize events.
         *
         * Adjusts the camera projection parameters when the viewport dimensions change.
         * - For perspective projection: Updates the aspect ratio.
         * - For orthographic projection: Adjusts the right and top boundaries to maintain
         *   the correct projection volume.
         *
         * @param width The new viewport width in pixels.
         * @param height The new viewport height in pixels.
         */
        void onResize(const float width, const float height) noexcept {

            if (usePerspective_) {
                setAspectRatio(width / height);
                return;
            }

            if (left_ == 0.0f && bottom_ == 0.0f) {
                right_ = width;
                top_ = height;
            } else {

                left_ = -width/2.0f;
                right_ = width/2.0f;
                bottom_ = -height/2.0f;
                top_ = height/2.0f;
            }

            needsUpdate_ = true;
        }

        /**
         * @brief Sets the perspective projection parameters.
         *
         * @param fovY The vertical field of view in radians.
         * @param aspectRatio The aspect ratio (width/height).
         * @param zNear The near clipping plane distance.
         * @param zFar The far clipping plane distance.
         *
         * @return A reference to this camera instance.
         */
        Camera& setPerspective(float fovY, float aspectRatio, float zNear, float zFar) noexcept {
            assert(zNear > 0 && "zNear must be positive");
            assert(zFar > 0 && zFar > zNear && "zFar must be positive and greater than zNear");
            fovY_ = fovY;
            aspectRatio_ = aspectRatio;
            zNear_ = zNear;
            zFar_ = zFar;
            needsUpdate_ = true;
            usePerspective_ = true;
            return *this;
        }

        /**
         * @brief Sets the orthographic projection parameters.
         *
         * Configures the camera for orthographic projection, which is useful for 2D rendering,
         * UI overlays, or scenes where parallel lines should remain parallel (no perspective
         * distortion).
         *
         * @param left The left boundary of the view volume.
         * @param right The right boundary of the view volume.
         * @param bottom The bottom boundary of the view volume.
         * @param top The top boundary of the view volume.
         * @param zNear The near clipping plane distance. Defaults to -1.0.
         * @param zFar The far clipping plane distance. Defaults to 100.0.
         *
         * @return A reference to this camera instance.
         *
         * @note Calling this method switches the camera to orthographic projection mode.
         */
        Camera& setOrtho(
            const float left , const float right,
            const float bottom, const float top,
            const float zNear = -1.0f, const float zFar = 100.0f) noexcept {

            assert(zFar > 0 && zFar > zNear && "zFar must be positive and greater than zNear");

            zNear_ = zNear;
            zFar_ = zFar;

            left_ = left;
            right_ = right;
            bottom_ = bottom;
            top_ = top;

            aspectRatio_ = (right - left) / (top - bottom); // w/h

            usePerspective_ = false;

            needsUpdate_ = true;
            return *this;
        }

        /**
         * @brief Sets the near clipping plane distance.
         *
         * @param zNear The new near clipping plane distance.
         *
         * @return A reference to this camera instance.
         */
        Camera& setZNear(float zNear) noexcept {
            zNear_ = zNear;
            needsUpdate_ = true;
            return *this;
        }

        /**
         * @brief Sets the far clipping plane distance.
         *
         * @param zFar The new far clipping plane distance.
         *
         * @return A reference to this camera instance.
         */
        Camera& setZFar(float zFar) noexcept {
            zFar_ = zFar;
            needsUpdate_ = true;
            return *this;
        }

        /**
         * @brief Sets the vertical field of view.
         *
         * @param fovY The new vertical field of view in radians.
         *
         * @return A reference to this camera instance.
         */
        Camera& setFovY(float fovY) noexcept {
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