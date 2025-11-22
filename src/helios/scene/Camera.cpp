module;

#include <memory>

module helios.scene.Camera;

import helios.math.types;
import helios.scene.SceneNode;

namespace helios::scene {

    Camera::Camera() noexcept :
        projectionMatrix_(helios::math::mat4f::identity()),
        viewMatrix_(helios::math::mat4f::identity())
    {}

    const helios::math::mat4f& Camera::projectionMatrix() const noexcept {
        return projectionMatrix_;
    }

    const helios::math::mat4f& Camera::viewMatrix() const noexcept {
        return viewMatrix_;
    }

    void Camera::setAspectRatio(const float aspectRatio) noexcept {
        aspectRatio_ = aspectRatio;
    }

} // namespace helios::scene
