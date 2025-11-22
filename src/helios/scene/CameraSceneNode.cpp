module;

#include <cassert>
#include <memory>
#include <stdexcept>

module helios.scene.CameraSceneNode;

import helios.math.types;
import helios.scene.SceneNode;
import helios.scene.Camera;

namespace helios::scene {

    CameraSceneNode::CameraSceneNode(std::shared_ptr<Camera> camera) :
        camera_(camera) {
        // intenionally test the shared_ptr,
        // we can check later if camera_ is expired
        if (!camera) {
            throw std::invalid_argument("CameraSceneNode received a Camera nullptr.");
        }
    }

    SceneNode* CameraSceneNode::addChild(std::unique_ptr<SceneNode> sceneNode) {
        assert(false && "CameraSceneNode does not accept child nodes.");
        return nullptr;
    }

    std::weak_ptr<Camera> CameraSceneNode::camera() const noexcept {
        return camera_;
    }

} // namespace helios::scene
