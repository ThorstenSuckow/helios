module;

#include <vector>

export module helios.scene.FrustumCullingStrategy;

import helios.scene.Camera;
import helios.scene.SceneNode;


export namespace helios::scene {

    /**
     * Abstract base class for implementing frustum culling strategies.
     *
     * A FrustumCullingStrategy determines which SceneNodes are visible, relative to
     * the view frustum of a given camera.
     *
     * @see [She07, pp. 443]
     *
     * @todo The returned list should be considered "as is" for now: Each entry in the
     * list a single renderable entity to consider. It child nodes (i.e. the sub tree **this** node is
     * the root of) should _not_ be automatically considered for rendering.
     * Later iterations should probably use an individual struct
     * that provides a hint whether for a given node a whole subtree should be considered,
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