module;

#include <vector>

import helios.scene.FrustumCullingStrategy;
import helios.scene.Camera;
import helios.scene.Scene;
import helios.scene.SceneNode;


module helios.scene.CullNoneStrategy;


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
        const helios::scene::Camera& camera, const helios::scene::Scene& scene
    )  {

        auto nodes = std::vector<const helios::scene::SceneNode*>();

        cull(nodes, scene.root());

        return nodes;
    }

}