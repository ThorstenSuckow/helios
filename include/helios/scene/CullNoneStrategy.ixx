/**
 * @file CullNoneStrategy.ixx
 * @brief Frustum culling strategy that returns all nodes (no culling).
 */
module;

#include <vector>

export module helios.scene.CullNoneStrategy;

import helios.scene.FrustumCullingStrategy;
import helios.scene.CameraSceneNode;
import helios.scene.SceneNode;

export namespace helios::scene {


    /**
     * @brief A default FrustumCullingStrategy that simply returns all
     * SceneNodes found, e.g. no culling is applied.
     */
    class CullNoneStrategy final : public helios::scene::FrustumCullingStrategy  {


    protected:
        /**
         * @brief Internal function to populate the vector recursively with the node and its children.
         *
         * @param nodes The vector holding the const pointers to the SceneNodes
         * @param node The current node to process. This node is added to nodes, then this
         * method is called for each child node of node.
         */
        void cull(
            std::vector<const helios::scene::SceneNode*>& nodes, const SceneNode& node
        ) const {
            if (node.isActive()) {
                nodes.push_back(&node);
            }
            for (auto& child: node.children()) {
                cull(nodes, *child);
            }
        }


    public:

        /**
         * @brief Returns all SceneNode of the specified scene, regardless of their visibility
         * relative to the CameraSceneNode's camera view frustum.
         *
         * @param cameraSceneNode The camera defining the view frustum.
         * @param root The parent of the hierarchy to cull.
         *
         * @return A vector with const pointers to the SceneNodes found under `root`.
         */
        [[nodiscard]] std::vector<const helios::scene::SceneNode*> cull(
            const helios::scene::CameraSceneNode* cameraSceneNode, const helios::scene::SceneNode& root
        ) override {

            auto nodes = std::vector<const helios::scene::SceneNode*>();

            cull(nodes, root);

            return nodes;
        }


    };
}