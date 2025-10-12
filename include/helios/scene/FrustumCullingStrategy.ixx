module;

#include <vector>

import helios.scene.Camera;
import helios.scene.SceneNode;

export module helios.scene.FrustumCullingStrategy;


export namespace helios::scene {

    /**
     * Abstract base class for implementing frustum culling strategies.
     *
     * A FrustumCullingStrategy determines which SceneNodes are visible, relative to
     * the view frustum of a given camera.
     *
     * @see [She07, pp. 443]
     *
     * @todo The returned list should be considered as is for now, i.e. one entry = one
     * renderable to consider. Later iterations should probably use an individual struct
     * that provide a hint whether for a given node a whole subtree should be considered,
     * saving time for culling and memory when constructing the list.
     */
    class FrustumCullingStrategy {


    public:

        virtual ~FrustumCullingStrategy() = default;

        FrustumCullingStrategy() = default;


        /**
         * Culls the scene graph to identify the visible SceneNodes.
         *
         * @param camera The camera defining the view frustum.
         * @param root The parent of the hierarchy to cull.
         *
         * @return A vector with const pointers to the SceneNodes visible.
         *
         * @node ownership of the SceneNode remains with their respective owners.
         * Implementing APIs have to make sure that the origins of the pointers returned with
         * the vector are not destroyed in between.
         */
        [[nodiscard]] virtual std::vector<const helios::scene::SceneNode*> cull(
            const helios::scene::Camera& camera, const helios::scene::SceneNode& root
        ) = 0;

    };



}