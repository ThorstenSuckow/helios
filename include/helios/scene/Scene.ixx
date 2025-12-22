/**
 * @file Scene.ixx
 * @brief Represents a hierarchical SceneGraph that organizes all renderable
 * and non-renderable objects within a scene.
 */
module;

#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

export module helios.scene.Scene;

import helios.scene.Snapshot;
import helios.scene.SceneNode;
import helios.scene.CameraSceneNode;
import helios.math.types;

import helios.rendering.Viewport;
import helios.scene.FrustumCullingStrategy;
import helios.scene.SnapshotItem;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::scene::Scene"
export namespace helios::scene {


    /**
     * @brief Represents a hierarchical SceneGraph that organizes all renderable
     * and non-renderable objects within a scene.
     *
     * Every scene has one implicit root node all nodes are appended to when
     * `addNode` is called.
     *
     * A Scene is responsible for managing the SceneGraph, consisting of SceneNodes;
     * it provides methods to frustum cull SceneNodes based on a specific `FrustumCullingStrategy`
     * and a particular `CameraSceneNode` and also allows for propagating world transformations
     * through the contained subtrees.
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
         * @brief SceneGraphKey for internal use.
         *
         * @see helios::scene::SceneNode::setWorldTransform()
         */
        const SceneGraphKey sceneGraphKey_{};

        /**
         * @brief 4x4 identity matrix for internal use.
         */
        const helios::math::mat4f mat4fid = helios::math::mat4f::identity();

        /**
         * @brief The root node of this scene.
         *
         * All other SceneNodes are (in)direct children of this node.
         */
        std::unique_ptr<SceneNode> root_;

        /**
         * @brief Internal helper function to force-propagate the worldTransformation of SceneNodes to their child nodes.
         *
         * Propagation is cancelled for this node if the resulting world transform
         * is considered equal to the current world transform of the SceneNode.
         *
         * @param node The current SceneNode for which to propagate the world transformation.
         * @param parentWorldTransform The world transformation from the parent node.
         *
         * @see updateNodes()
         */
        void propagateWorldTransform(SceneNode& node, const math::mat4f& parentWorldTransform) const;

        /**
         * @brief Selectively updates the world transformation of SceneNodes in the scene graph.
         *
         * A SceneNode is forced to update its own world transformation and the
         * world transformation of its children as soon as a check for the dirty-state of the
         * node is true.
         *
         * @param node The current SceneNode processed.
         * @param wt The world transform to apply.
         *
         * @note It is important that this method is called before the next render pass, or before
         * the scene graph is culled. This method must also consider CameraSceneNodes, making sure the
         * associated `Camera` is updated with the view matrix.
         */
        void updateNodes(SceneNode& node, const math::mat4f& wt) const;

        /**
         * @brief The FrustumCullingStrategy used with this Scene.
         */
        std::unique_ptr<helios::scene::FrustumCullingStrategy> frustumCullingStrategy_;

        /**
         * @brief The logger used with this Scene instance.
         *
         * Defaults to HELIOS_LOG_SCOPE.
         *
         * @todo constructor injection
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:
        /**
         * @brief Prevent copying.
         *
         * A Scene is not intended to be copied.
         */
        Scene(const Scene&) = delete;

        /**
         * @brief Prevent copy assignment.
         *
         * A Scene is not intended to be copied.
         */
        Scene& operator=(const Scene&) = delete;

        /**
         * @brief Prevent move constructor.
         */
        Scene(Scene&&) noexcept = delete;

        /**
         * @brief Prevent move assignment operator.
         */
        Scene& operator=(Scene&&) noexcept = delete;

        ~Scene() = default;

        /**
         * @brief Constructs a new Scene.
         *
         * The root node for this scene is automatically created.
         *
         * @param frustumCullingStrategy The frustum culling strategy to use with this Scene.
         */
        explicit Scene(std::unique_ptr<helios::scene::FrustumCullingStrategy> frustumCullingStrategy);


        /**
         * @brief Adds a new SceneNode to this scene.
         *
         * The SceneNode becomes a direct descendant of this scene's root.
         *
         * @param node The SceneNode to add to this Scene.
         *
         * @return The raw pointer to the newly added node, or `nullptr` if the node was not added.
         */
        [[nodiscard]] SceneNode* addNode(std::unique_ptr<SceneNode> node) const;

        /**
         * @brief Updates the world transformations of SceneNodes in the graph.
         *
         * The method traverses the Scene and propagates updated world transformations
         * to their respective child nodes if the processed node is considered to be dirty.
         */
        void updateNodes() const;

        /**
         * @brief Applies this Scene's frustumCullingStrategy and returns all nodes visible for the specified
         * CameraSceneNode and its associated `Camera`.
         *
         * This method ensures that all nodes' world geometry is properly updated
         * before culling is applied.
         *
         * @param cameraSceneNode The cameraSceneNode used to determine the view frustum for culling.
         *
         * @return A list of currently visible nodes (const pointers into the scene graph).
         */
        [[nodiscard]] std::vector<const helios::scene::SceneNode*> findVisibleNodes(
            const helios::scene::CameraSceneNode* cameraSceneNode
        ) const;

        /**
         * @brief Returns the root node of this Scene.
         *
         * The root node is guaranteed to exist.
         *
         * @return The root node of this Scene.
         */
        [[nodiscard]] helios::scene::SceneNode& root() const noexcept;

        /**
         * @brief Creates a Snapshot of the Scene and returns it.
         *
         * Taking a snapshot will frustum cull the SceneNodes and place their Renderables
         * along with the current matrices of this scene into the Snapshot object.
         * Only those SceneNodes are considered that were configured with a Renderable and
         * have their `isActive`-flag set to true.
         *
         * @param viewport The viewport for which the Snapshot is created.
         *
         * @return An optional Snapshot created for this Scene, or `std::nullopt` if creation failed.
         *
         * @todo This should be refactored into a factory to prevent domain leakage between Scene and Rendering.
         */
        [[nodiscard]] std::optional<Snapshot>
        createSnapshot(const std::shared_ptr<const rendering::Viewport>& viewport) const;

    };

} // namespace helios::scene

