module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

module helios.scene.Scene;

import helios.scene.SceneNode;
import helios.rendering.RenderQueue;
import helios.rendering.RenderableContext;
import helios.math.types;

import helios.scene.Camera;
import helios.scene.FrustumCullingStrategy;


namespace helios::scene {


    void Scene::propagateWorldTransform(SceneNode& node, const math::mat4f& wt) const {

        const auto& parentWt = wt * node.localTransform().transform();

        // if the worldTransform was not updated,
        // branch back into selective updates
        if (!node.setWorldTransform(parentWt, sceneGraphKey_)) {
            for (auto& child: node.children()) {
                updateNodes(*child, parentWt);
            }
        } else {
            for (auto& child: node.children()) {
                propagateWorldTransform(*child, parentWt);
            }
        }

    }


    void Scene::updateNodes(SceneNode& node, const math::mat4f& wt) const {

        if (node.needsUpdate()) {
            propagateWorldTransform(node, wt);
        } else {
            const auto& parentWt = node.worldTransform();
            for (auto& child: node.children()) {
                updateNodes(*child, parentWt);
            }
        }
    }

    Scene::Scene(std::unique_ptr<helios::scene::FrustumCullingStrategy> frustumCullingStrategy) :
    sceneGraphKey_(),
    frustumCullingStrategy_(std::move(frustumCullingStrategy)),
    root_(std::make_unique<SceneNode>()) {}



    SceneNode& Scene::addNode(std::unique_ptr<SceneNode> node) const {
        assert(root && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }
        return root_->addChild(std::move(node));
    }


    void Scene::updateNodes() const {
        updateNodes(*root_, mat4fid);
    }


    [[nodiscard]] std::vector<const helios::scene::SceneNode*> Scene::findVisibleNodes(const helios::scene::Camera& camera) const {
        assert(root && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }

        updateNodes();

        return frustumCullingStrategy_->cull(camera, *root_);
    }


    [[nodiscard]] helios::scene::SceneNode& Scene::root() const noexcept {
        assert(root && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }
        return *root_;
    }

}