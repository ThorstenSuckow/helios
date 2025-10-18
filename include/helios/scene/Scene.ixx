module;

#include <memory>
#include <stdexcept>
#include <vector>

export module helios.scene.Scene;

import helios.scene.Snapshot;
import helios.scene.SceneNode;
import helios.math.types;

import helios.scene.Camera;
import helios.scene.FrustumCullingStrategy;
import helios.scene.SnapshotItem;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::scene::Scene"
export namespace helios::scene {


    /**
     * Represents a hierarchical SceneGraph that organizes all renderable
     * and non-renderable objects within a scene.
     *
     * Every scene has one implicit root node all nodes are appended to when
     * `addNode` is called.
     *
     * A Scene is responsible for managing the SceneGraph, consisting of SceneNodes;
     * it provides methods to frustum cull SceneNodes based on a specific `FrustumCullingStrategy`
     * and a particular `Camera` and also allows for propagating world transformations
     * through the contained subtrees.
     *
     *
     * @see [RTR, pp. 828]
     * @see [Gre19, pp. 693]
     * @see [HDMS+14, pp. 138]
     *
     * @note By default, we're not allowing copying/moving Scenes.
     * These constraints may be relaxed in the future, depending on valid use cases.
     */
    class Scene {

    private:

        /**
         * SceneGraphKey for internal use.
         * @see helios::scene::SceneNode::setWorldTransform()
         */
        const SceneGraphKey sceneGraphKey_{};

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
        /**
         * Prevent copying.
         * A Scene is not intended to be copied.
         */
        Scene(const Scene&)=delete;

        /**
         * Prevent copy assignment.
         * A scene is not intended to be copied.
         */
        Scene& operator=(const Scene&)=delete;

        /**
         * Prevent move constructor.
         */
        Scene(Scene&&) noexcept = delete;

        /**
         * Prevent move assignment operator.
         */
        Scene& operator=(Scene&&) noexcept = delete;

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
         * @return The raw pointer to the newly added node, or `nullptr` if
         * the was not added.
         *
         */
        [[nodiscard]] SceneNode* addNode(std::unique_ptr<SceneNode> node) const;

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

        /**
         * Creates a Snapshot of the Scene and returns it.
         * Taking a snapshot will frustum cull the SceneNodes and place their Renderables
         * along with the current matrices of this scene's matrix into the Snapshot object.
         *
         * @param camera The camera used to observe the Scene.
         *
         * @return The snapshot created for this Scene.
         */
        [[nodiscard]] Snapshot createSnapshot(const helios::scene::Camera& camera) const;

    };

}