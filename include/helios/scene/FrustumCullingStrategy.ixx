/**
 * @file FrustumCullingStrategy.ixx
 * @brief Abstract base class for implementing frustum culling strategies.
 */
module;

#include <vector>

export module helios.scene.FrustumCullingStrategy;

import helios.scene.CameraSceneNode;
import helios.scene.SceneNode;


export namespace helios::scene {

    /**
     * @brief Abstract base class for implementing frustum culling strategies.
     *
     * A FrustumCullingStrategy determines which SceneNodes are visible, relative to
     * the view frustum of a given camera.
     *
     * @see [She07, pp. 443]
     *
     * @todo The returned list should be considered "as is" for now: Each entry in the
     * list is a single renderable entity to consider. Its child nodes (i.e. the sub tree for
     * which this node is the root) should _not_ be automatically considered for rendering.
     * Later iterations should probably use an individual struct
     * that provides a hint whether for a given node a whole subtree should be considered,
     * saving time for culling and memory when constructing the list.
     */
    class FrustumCullingStrategy {


    public:

        virtual ~FrustumCullingStrategy() = default;

        FrustumCullingStrategy() = default;


        /**
         * @brief Culls the scene graph to identify the visible SceneNodes.
         *
         * @param cameraSceneNode The CameraSceneNode providing information about the view frustum.
         * @param root The parent of the hierarchy to cull.
         *
         * @return A vector with const pointers to the SceneNodes visible.
         *
         * @note Ownership of the SceneNodes remains with their respective owners.
         * Implementing APIs have to make sure that the origins of the pointers returned with
         * the vector are not destroyed in between.
         */
        [[nodiscard]] virtual std::vector<const helios::scene::SceneNode*> cull(
            const helios::scene::CameraSceneNode* cameraSceneNode, const helios::scene::SceneNode& root
        ) = 0;

    };



}