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
     */
    class CameraSceneNode : public SceneNode {



    protected:
        /**
         * @brief Unique pointer to the camera instance owned by this scene node.
         */
        std::unique_ptr<helios::scene::Camera> camera_;

    public:
        /**
         * @brief Constructs a CameraSceneNode that takes ownership of the given camera.
         *
         * The CameraSceneNode becomes the sole owner of the Camera instance. The camera's
         * view matrix is automatically updated when `worldTransform()` is called.
         *
         * @param camera A unique pointer to the camera to be owned by this scene node.
         *
         * @throws std::invalid_argument if camera is a nullptr.
         */
        explicit CameraSceneNode(std::unique_ptr<helios::scene::Camera> camera);

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
        [[nodiscard]] SceneNode* addNode(std::unique_ptr<SceneNode> sceneNode) override;

        /**
         * @brief Gets the camera associated with this scene node.
         *
         * @return A const ref to the associated camera instance.
         */
        [[nodiscard]] const helios::scene::Camera& camera() const noexcept;

        /**
         * @brief Gets the camera associated with this scene node.
         *
         * @return A ref to the associated camera instance.
         */
        [[nodiscard]] helios::scene::Camera& camera() noexcept;


        /**
         * @brief Orients this camera node to look at a target position in world space.
         *
         * Computes the rotation matrix required to align this scene node's forward direction
         * with the specified target position. The rotation is computed in world space and then
         * converted to local space by factoring out the parent's rotation.
         *
         * @param target The world-space coordinates of the target to look at.
         * @param up The up vector defining the camera's vertical orientation in world space.
         *
         * @pre The camera node must have a valid parent (i.e., must be added to a scene graph).
         *
         * @note The up vector should typically be `(0, 1, 0)` for standard upright orientation.
         *
         * @see lookAtLocal()
         * @see helios::scene::SceneNode::rotate()
         */
        void lookAt(helios::math::vec3f target, helios::math::vec3f up);

        /**
         * @brief Orients this camera node to look at a target position in local space.
         *
         * Similar to `lookAt()`, but the target and up vector are interpreted relative
         * to this node's local coordinate system rather than world space. Useful when
         * the camera should track objects within the same local hierarchy.
         *
         * @param targetLocal The local-space coordinates of the target to look at.
         * @param upLocal The up vector defining the camera's vertical orientation in local space.
         *
         * @see lookAt()
         */
        void lookAtLocal(helios::math::vec3f targetLocal, helios::math::vec3f upLocal);

        /**
         * @brief Callback invoked when the world transform of this node changes.
         *
         * Overrides the base class implementation to update the camera's view matrix
         * whenever the node's position or orientation in the scene graph changes.
         * This ensures the camera always reflects the current world-space transform.
         *
         * @see SceneNode::onWorldTransformUpdate()
         */
        void onWorldTransformUpdate() noexcept override;
    };

}
