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
        [[nodiscard]] SceneNode* addChild(std::unique_ptr<SceneNode> sceneNode) override;

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
         * @todo Implement `lookAtLocal()` for observing nodes in local space.
         *
         * @see helios::scene::SceneNode::rotate()
         */
        void lookAt(helios::math::vec3f target, helios::math::vec3f up);


        /**
         * @brief Computes the world transform and updates the associated camera's view matrix.
         *
         * This override extends `SceneNode::worldTransform()` by additionally computing the
         * view matrix from the camera's world transform. The view matrix is the inverse of the
         * world transform, with the Z-axis negated to convert from helios' Left-Handed System
         * to OpenGL's Right-Handed clip space.
         *
         * The computed view matrix is automatically assigned to the underlying `Camera` instance.
         *
         * @return A const reference to this node's world transform matrix.
         *
         * @see helios::scene::Camera::setViewMatrix()
         * @see helios::scene::SceneNode::worldTransform()
         */
        const helios::math::mat4f& worldTransform() noexcept override;
    };

}
