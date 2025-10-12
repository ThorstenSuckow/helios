module;

#include <vector>

import helios.scene.FrustumCullingStrategy;
import helios.scene.Camera;
import helios.scene.SceneNode;


export module helios.scene.CullNoneStrategy;


export namespace helios::scene {


    /**
     * A default FrustumCullingStrategy that simply returns all
     * SceneNodes found, e.g. no culling is applied.
     */
    class CullNoneStrategy final : public helios::scene::FrustumCullingStrategy  {


    protected:
        /**
         * Internal function to populate the vector recursively with the node and its children.
         *
         * @param nodes The vector holding the const pointers to the SceneNodes
         * @param node The current node to process. This node is added to nodes, then this
         * method is called for each child node of node.
         */
        void cull(
            std::vector<const helios::scene::SceneNode*>& nodes, const SceneNode& node
        ) const;


    public:

        /**
         * Returns all SceneNode of the specified scenes, regardless of their visibility
         * relative to the camera's view frustum.
         *
         * @param camera The camera defining the view frustum.
         * @param root The parent of the hierarchy to cull.
         *
         * @return A vector with const pointers to the SceneNodes visible.
         */
        [[nodiscard]] std::vector<const helios::scene::SceneNode*> cull(
            const helios::scene::Camera& camera, const helios::scene::SceneNode& root
        ) override;


    };



}