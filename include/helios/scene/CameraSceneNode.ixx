/**
 * @file CameraSceneNode.ixx
 * @brief Camera scene node adapter providing view and projection matrices within a scene graph.
 */
module;

#include <memory>

export module helios.scene.CameraSceneNode;

import helios.math.types;
import helios.scene.SceneNode;
import helios.scene.Camera;

export namespace helios::scene {

    /**
     * @brief Represents an adapter for cameras, allowing spatial positioning and transformation within the scene graph.
     *
     * A CameraSceneNode behaves just like a (non-renderable) SceneNode, except that
     * it does not accept child nodes.
     *
     * An instance of `CameraSceneNode` can be added as a direct descendant of the scene's root node
     * for free transform, or it can be a child of a model hierarchy to inherit the
     * corresponding positioning in the world.
     *
     * @todo Check whether nodes should further be divided into ParentNode and LeafNode types.
     *       Cameras would be LeafNodes, not allowing child nodes.
     */
    class CameraSceneNode : public SceneNode {

    protected:
        /**
         * @brief Weak pointer to the camera instance managed by this scene node.
         */
        std::weak_ptr<helios::scene::Camera> camera_;

    public:
        /**
         * @brief Constructs a CameraSceneNode wrapping the given camera.
         *
         * The Camera must be a non-null shared pointer, it gets internally stored as a
         * weak_ptr.
         *
         * @param camera A shared pointer to the camera to be managed by this scene node.
         *
         * @throws std::invalid_argument if camera is a nullptr
         */
        explicit CameraSceneNode(std::shared_ptr<helios::scene::Camera> camera);

        /**
         * @brief Prevents adding child nodes to a camera scene node.
         *
         * This method does nothing and returns `nullptr`, indicating that a `CameraSceneNode`
         * must not have child nodes.
         *
         * @param sceneNode The attempted child node to add (ignored).
         *
         * @return Always returns `nullptr`.
         *
         * @see https://stackoverflow.com/questions/24609872/delete-virtual-function-from-a-derived-class
         */
        [[nodiscard]] SceneNode* addChild(std::unique_ptr<SceneNode> sceneNode) override;

        /**
         * @brief Gets the camera associated with this scene node.
         *
         * @return A weak pointer to the camera instance.
         */
        [[nodiscard]] std::weak_ptr<helios::scene::Camera> camera() const noexcept;

        /**
         * @todo once Cameras should be used with parent nodes, this method needs to be implemented.
         */

    };

}
