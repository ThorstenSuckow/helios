module;

#include <vector>

module helios.scene.CullNoneStrategy;

import helios.scene.FrustumCullingStrategy;
import helios.scene.Camera;
import helios.scene.Scene;
import helios.scene.SceneNode;

namespace helios::scene {

    void CullNoneStrategy::cull(
        std::vector<const helios::scene::SceneNode*>& nodes, const SceneNode& node
    ) const {
        nodes.push_back(&node);
        for (auto& child: node.children()) {
            cull(nodes, *child);
        }
    }

    [[nodiscard]] std::vector<const helios::scene::SceneNode*> CullNoneStrategy::cull(
        const helios::scene::Camera& camera, const helios::scene::SceneNode& root
    )  {

        auto nodes = std::vector<const helios::scene::SceneNode*>();

        cull(nodes, root);

        return nodes;
    }

}