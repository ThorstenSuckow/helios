module;

#include <cassert>

module helios.scene.Camera;

import helios.math.types;
import helios.math.transform;
import helios.scene.SceneNode;

namespace helios::scene {

    Camera::Camera() noexcept :
        projectionMatrix_(helios::math::mat4f::identity()),
        viewMatrix_(helios::math::mat4f::identity())
    {}

    const helios::math::mat4f& Camera::projectionMatrix() const noexcept {
        update();
        return projectionMatrix_;
    }

    const helios::math::mat4f& Camera::viewMatrix() const noexcept {
        return viewMatrix_;
    }

    const Camera& Camera::setViewMatrix(const helios::math::mat4f& viewMatrix) const noexcept {
        viewMatrix_ = viewMatrix;
        return *this;
    }

    const Camera& Camera::setAspectRatio(const float aspectRatio) const noexcept {
        needsUpdate_ = true;
        aspectRatio_ = aspectRatio;
        return *this;
    }

    const Camera& Camera::setPerspective(const float fovY, const float aspectRatio, const float zNear, const float zFar) const noexcept {
        assert(zNear > 0 && "zNear must be positive");
        assert(zFar > 0 && zFar > zNear && "zFar must be positive and greater than zNear");
        fovY_ = fovY;
        aspectRatio_ = aspectRatio;
        zNear_ = zNear;
        zFar_ = zFar;
        needsUpdate_ = true;
        return *this;
    }

    const Camera& Camera::setZNear(const float zNear) const noexcept {
        zNear_ = zNear;
        needsUpdate_ = true;
        return *this;
    }

    const Camera& Camera::setZFar(const float zFar) const noexcept {
        zFar_ = zFar;
        needsUpdate_ = true;
        return *this;
    }

    const Camera& Camera::setFovY(const float fovY) const noexcept {
        fovY_ = fovY;
        needsUpdate_ = true;
        return *this;
    }

    float Camera::fovY() const noexcept {
        return fovY_;
    }

    float Camera::aspectRatio() const noexcept {
        return aspectRatio_;
    }

    float Camera::zNear() const noexcept {
        return zNear_;
    }

    float Camera::zFar() const noexcept {
        return zFar_;
    }

} // namespace helios::scene
