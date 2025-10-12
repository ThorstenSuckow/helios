module;

#include <vector>

import helios.scene.Camera;
import helios.scene.Scene;
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
     */
    class FrustumCullingStrategy {


    public:

        virtual ~FrustumCullingStrategy() = default;

        FrustumCullingStrategy() = default;


        /**
         * Culls the scene graph to identify the visible SceneNodes.
         *
         * @param camera The camera defining the view frustum.
         * @param root The Scene to be culled.
         *
         * @return A vector with const pointers to the SceneNodes visible.
         *
         * @node ownership of the SceneNode remains with their respective owners.
         * Implementing APIs have to make sure that the origins of the pointers returned with
         * the vector are not destroyed in between.
         */
        [[nodiscard]] virtual std::vector<const helios::scene::SceneNode*> cull(
            const helios::scene::Camera& camera, const helios::scene::Scene& root
        ) = 0;

    };



}