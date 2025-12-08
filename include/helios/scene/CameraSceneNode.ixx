/**
 * @file CameraSceneNode.ixx
 * @brief Camera scene node adapter providing view and projection matrices within a scene graph.
 */
module;

#include <cassert>
#include <iostream>
#include <memory>
#include <ostream>

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
         * @brief Constructs a CameraSceneNode wrapping the given camera.
         *
         * The Camera must be a non-null shared pointer, it gets internally stored as a
         * weak_ptr.
         *
         * @param camera A shared pointer to the camera to be managed by this scene node.
         *
         * @throws std::invalid_argument if camera is a nullptr
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
         * Computes the rotation matrix for this Scene node to align this scene node
         * with the target, given the specified up vector.
         *
         * @param target world coordinates of the target to observe.
         * @param up
         *
         * @todo implement lookAtLocal for observing nodes in the local space
         *
         * @note make sure lookAt() is called
         */
        void lookAt(helios::math::vec3f target, helios::math::vec3f up) {

            // we treat up as a vector in world-space for now.

            const auto wt = worldTransform();
            const auto worldPos = helios::math::vec3f(
                wt(0, 3), wt(1, 3), wt(2, 3)
            );

            auto z = (target - worldPos).normalize();
            auto y = up.normalize();
            auto x = helios::math::cross(y, z).normalize();
            y = helios::math::cross(z, x).normalize();

            const helios::math::mat4f worldRotation = helios::math::mat4f{
                x[0], x[1], x[2], 0.0f,
                 y[0], y[1],  y[2],  0.0f,
                z[0], z[1], z[2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            // ParentTransform
            assert(parent() && "parent() of CameraSceneNode returned null, are you sure the node was added properly to teh scenegraph?");
            const helios::math::mat4f pT = parent()->worldTransform();

            // Transpose of parent rotation - needed to undo the rotation for
            // **this** node
            const helios::math::mat4f parentRotationT = helios::math::mat4f{
                pT(0, 0), pT(1, 0), pT(2, 0), 0.0f,
                pT(0, 1), pT(1, 1), pT(2, 1), 0.0f,
                pT(0, 2), pT(1, 2), pT(2, 2), 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            // apply rotation in local space by undoing the parent rotation
            rotate(parentRotationT * worldRotation);
        }


        /**
         * The world transform takes care of computing the view matrix, and assigns the value to the underlying
         * @return
         */
        const helios::math::mat4f& worldTransform() noexcept override {

            // updates this SceneNode's worldTransform_
            const auto wt = helios::scene::SceneNode::worldTransform();

            const auto x = helios::math::vec3f{wt(0, 0), wt(1, 0), wt(2, 0)};
            const auto y = helios::math::vec3f{wt(0, 1), wt(1, 1), wt(2, 1)};
            const auto z = helios::math::vec3f{wt(0, 2), wt(1, 2), wt(2, 2)};

            const auto eye = helios::math::vec3f{wt(0, 3), wt(1, 3),  wt(2, 3)};

            auto inv = helios::math::mat4f{
                    x[0], y[0], -z[0], 0.0f,
                    x[1], y[1], -z[1], 0.0f,
                    x[2], y[2], -z[2], 0.0f,
                    -dot(x, eye), -dot(y, eye), dot(z, eye), 1.0f

                };
                std::ignore = camera_->setViewMatrix(inv);

            return worldTransform_;
        }
    };

}
