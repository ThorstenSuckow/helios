module;

#include <memory>

export module helios.scene.Camera;

import helios.math.types;
import helios.scene.SceneNode;

export namespace helios::scene {

    /**
     * Represents a camera for perspective or orthographic projection, deriving from SceneNode
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
         * The projection matrix of this Camera.
         */
        helios::math::mat4f projectionMatrix_;

        /**
         * The view matrix of this Camera.
         */
        helios::math::mat4f viewMatrix_;

    public:

        /**
         * Constructor for this class.
         * Initializes projectionMatrix and viewMatrix with the identity matrix.
         */
        Camera();

        /**
         * This method will do nothing but returning a `nullptr`, indicating that a `Camera`
         * must not have child modes.
         *
         * @param sceneNode
         *
         * @return This implementation returns a `nullptr.`
         *
         * @see https://stackoverflow.com/questions/24609872/delete-virtual-function-from-a-derived-class
         */
        [[nodiscard]] SceneNode* addChild(std::unique_ptr<SceneNode> sceneNode) override;

        /**
         * Returns a constant ref to this camera's current projection matrix.
         * Typically set to the perspective / orthographic projection.
         *
         * @return A const ref to this `Camera`'s projection matrix.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() noexcept;

        /**
         * Returns a constant ref to this camera' view matrix.
         * Automatically computed as inverse from the camera's world
         * transform (i.e. [v]_c = P_{C <- W } [v]_w) = P_{W <- C}^{-1} [v]_w
         *
         * @return A const ref to this `Camera`s view matrix.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() noexcept;

    };


}
