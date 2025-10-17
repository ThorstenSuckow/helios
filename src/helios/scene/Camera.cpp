module;

#include <memory>

module helios.scene.Camera;

import helios.math.types;
import helios.scene.SceneNode;

namespace helios::scene {

     Camera::Camera() :
        projectionMatrix_(helios::math::mat4f::identity()),
        viewMatrix_(helios::math::mat4f::identity())
    {}

    const helios::math::mat4f& Camera::projectionMatrix() noexcept {
        return projectionMatrix_;
    }

    const helios::math::mat4f& Camera::viewMatrix() noexcept {
        return viewMatrix_;
    }

    SceneNode* Camera::addChild(std::unique_ptr<SceneNode> sceneNode) {
        return nullptr;
    };

}
