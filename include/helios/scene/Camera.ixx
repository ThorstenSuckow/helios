/**
 * @brief Camera node providing view and projection matrices.
 */
module;

#include <memory>

export module helios.scene.Camera;

import helios.math.types;
import helios.scene.SceneNode;

export namespace helios::scene {

    /**
     * @brief Represents a camera for perspective or orthographic projection, deriving from SceneNode
     * to allow for spatial positioning and transformation within the scene graph.
     *
     * The camera provides the view and projection matrix, whereas the view matrix
     * is computed as the inverse of the camera's world transform.
     *
     * A Camera behaves just like a (non-renderable) SceneNode, except for the fact that
     * it does not accept child nodes.
     *
     * An instance of `Camera` is added as a direct descendant of the scene's root node
     * for free transform. It can be a child of a model hierarchy to inherit the
     * corresponding positioning in the world.
     *
     * @todo implement class. Make sure localTransforms mark this class as dirty,
     * for proper recalculating projection-/viewMatrix if needed.
     * @todo check whether nodes should further be divided into Parent- and LeafNodes.
     * Cameras would be LeafNodes then, not allowing to have child nodes.
     */
    class Camera : public SceneNode {

    protected:
        /**
         * @brief The projection matrix of this Camera.
         */
        mutable helios::math::mat4f projectionMatrix_;

        /**
         * @brief The view matrix of this Camera.
         */
        mutable helios::math::mat4f viewMatrix_;

    public:
        /**
         * @brief Constructor for this class.
         * Initializes projectionMatrix and viewMatrix with the identity matrix.
         */
        Camera();

        /**
         * @brief This method will do nothing but return `nullptr`, indicating that a `Camera`
         * must not have child nodes.
         *
         * @param sceneNode The attempted child node to add (ignored).
         * @return This implementation returns `nullptr`.
         *
         * @see https://stackoverflow.com/questions/24609872/delete-virtual-function-from-a-derived-class
         */
        [[nodiscard]] SceneNode* addChild(std::unique_ptr<SceneNode> sceneNode) override;

        /**
         * @brief Returns a constant ref to this camera's current projection matrix.
         * Typically set to the perspective / orthographic projection.
         *
         * @return A const ref to this `Camera`'s projection matrix.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() const noexcept;

        /**
         * @brief Returns a constant ref to this camera's view matrix.
         * Automatically computed as the inverse of the camera's world
         * transform.
         *
         * @return A const ref to this `Camera`'s view matrix.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() const noexcept;

    };


}
