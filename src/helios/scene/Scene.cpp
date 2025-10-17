module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>
#include <iostream>

module helios.scene.Scene;

import helios.scene.SceneNode;

import helios.rendering.Renderable;
import helios.rendering.RenderQueue;
import helios.rendering.RenderCommand;
import helios.math.types;

import helios.scene.Camera;
import helios.scene.FrustumCullingStrategy;
import helios.scene.SnapshotRenderItem;


using namespace helios::math;
using namespace helios::rendering;

namespace helios::scene {


    void Scene::propagateWorldTransform(SceneNode& node, const mat4f& wt) const {

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


    void Scene::updateNodes(SceneNode& node, const mat4f& wt) const {

        if (node.needsUpdate()) {
            propagateWorldTransform(node, wt);
        } else {
            const auto& parentWt = node.worldTransform();
            for (auto& child: node.children()) {
                updateNodes(*child, parentWt);
            }
        }
    }

    Scene::Scene(std::unique_ptr<FrustumCullingStrategy> frustumCullingStrategy) :
    frustumCullingStrategy_(std::move(frustumCullingStrategy)),
    root_(std::make_unique<SceneNode>()) {}



    SceneNode* Scene::addNode(std::unique_ptr<SceneNode> node) const {
        assert(root_ && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }
        return root_->addChild(std::move(node));
    }


    void Scene::updateNodes() const {
        updateNodes(*root_, mat4fid);
    }


    std::vector<const SceneNode*> Scene::findVisibleNodes(const Camera& camera) const {
        assert(root_ && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }

        updateNodes();

        return frustumCullingStrategy_->cull(camera, *root_);
    }


    SceneNode& Scene::root() const noexcept {
        assert(root_ && "Unexpected null-root");
        if (!root_) {
            logger_.error("Unexpected nullptr for this Scene's root.");
        }
        return *root_;
    }

    Snapshot Scene::createSnapshot(const Camera& camera) const {
        const auto nodes = findVisibleNodes(camera);

        std::vector<SnapshotRenderItem> renderables;
        renderables.reserve(nodes.size());
        for (const auto& node : nodes) {
            if (node->renderable()) {
                renderables.emplace_back(
                    node->renderable(),
                    node->cachedWorldTransform()
                );
            }
        }

        return {
            camera.projectionMatrix(),
            camera.viewMatrix(),
            std::move(renderables)
        };
    }

}