module;

#include <cassert>
#include <memory>
#include <stdexcept>
#include <vector>

export module helios.scene.Scene;

import helios.scene.SceneNode;
import helios.rendering.RenderQueue;
import helios.rendering.RenderableContext;
import helios.math.types;

import helios.scene.Camera;
import helios.scene.FrustumCullingStrategy;

#define HELIOS_LOG_SCOPE "helios::scene::Scene"
export namespace helios::scene {


    /**
     * Represents a hierarchical SceneGraph that organizes all renderable
     * and non-renderable objects within a scene.
     *
     * Every scene has one implicit root node all nodes are appended to when
     * `addNode` is called.
     *
     *
     * @see [RTR, pp. 828], [Gre19, pp. 693], [HDMS+14, pp. 138]
     */
    class Scene {

    private:

        /**
         * SceneGraphKey for internal use.
         * see helios::scene::SceneNode::setWorldTransform()
         */
        const SceneGraphKey sceneGraphKey_;

        /**
         * 4x4 identity matrix for internal use.
         */
        const helios::math::mat4f mat4fid = helios::math::mat4f::identity();

        /**
         * The root node of this scene. All other SceneNodes are (in)direct
         * children of this node.
         */
        std::unique_ptr<SceneNode> root_;

        /**
         * Internal helper function to force-propagate the worldTransformation of SceneNodes
         * to their child nodes.
         * Propagation is cancelled for this node if the world transform submitted
         * is considered equal to the current world transform of the SceneNode.
         *
         * @param node The current SceneNode for which to propagate the world transformation.
         * @param wt The world transformation from the parent node.
         *
         * @see updateNodes()
         */
        void propagateWorldTransform(SceneNode& node, const math::mat4f& wt) const;

        /**
         * Selectively updates the world transformation of SceneNodes in the scene graph.
         * A SceneNode is forced to update its own world transformation and the
         * world transformation of its children as soon as a check for the dirty-state of the
         * node is true.
         *
         * @param node The current SceneNode processed.
         * @param wt The world transform to apply.
         */
        void updateNodes(SceneNode& node, const math::mat4f& wt) const;



    protected:
        /**
         * The FrustumCullingStrategy used with this Scene.
         */
        std::unique_ptr<helios::scene::FrustumCullingStrategy> frustumCullingStrategy_;

        /**
         * The logger used with this Scene instance.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        ~Scene() = default;

        /**
         * Constructs a new Scene.
         * The root node for this scene is automatically created.
         */
        explicit Scene(std::unique_ptr<helios::scene::FrustumCullingStrategy> frustumCullingStrategy);


        /**
         * Adds a new SceneNode to this scene.
         * The SceneNode becomes a direct descendant of this scene's root.
         *
         * @param node The Scene node to add to this Scene.
         *
         * @return A reference to the **newly added SceneNode**.
         *
         */
        SceneNode& addNode(std::unique_ptr<SceneNode> node) const;

        /**
         * Updates the world transformations of SceneNodes in the graph.
         * The method traverses the Scene and propagates updated world transformations
         * to their respective child nodes if the processed node is considered to be dirty.
         */
        void updateNodes() const;

        /**
         * This method applies this Scene's frustumCullingStrategy and returns all nodes
         * visible for the specified Camera.
         * This method makes also sure that all nodes' world geometry is properly updated
         * before culling is applied.
         *
         * @param camera The
         *
         * @return a list of currently visible nodes
         */
        [[nodiscard]] std::vector<const helios::scene::SceneNode*> findVisibleNodes(const helios::scene::Camera& camera) const;

        /**
         * Returns the root not of this Scene.
         * Thr root node is considered to be always existing.
         * @return the root node of this Scene.
         */
        [[nodiscard]] helios::scene::SceneNode& root() const noexcept;

    };

}