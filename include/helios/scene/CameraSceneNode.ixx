/**
 * @file CameraSceneNode.ixx
 * @brief Camera scene node adapter providing view and projection matrices within a scene graph.
 */
module;

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

export module helios.scene.CameraSceneNode;

import helios.math.types;
import helios.math.TransformType;
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
        explicit CameraSceneNode(std::unique_ptr<helios::scene::Camera> camera) :
            camera_(std::move(camera)) {
            if (!camera_) {
                throw std::invalid_argument("CameraSceneNode received a Camera nullptr.");
            }
        }

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
        [[nodiscard]] SceneNode* addNode(std::unique_ptr<SceneNode> sceneNode) override {
            assert(false && "CameraSceneNode does not accept child nodes.");
            return nullptr;
        }

        /**
         * @brief Gets the camera associated with this scene node.
         *
         * @return A const ref to the associated camera instance.
         */
        [[nodiscard]] const helios::scene::Camera& camera() const noexcept {
            return *camera_;
        }

        /**
         * @brief Gets the camera associated with this scene node.
         *
         * @return A ref to the associated camera instance.
         */
        [[nodiscard]] helios::scene::Camera& camera() noexcept {
            return *camera_;
        }


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
         * @see helios::scene::SceneNode::setRotation()
         */
        void lookAt(helios::math::vec3f target, helios::math::vec3f up) {
            // We treat up as a vector in world-space for now.
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
                y[0], y[1], y[2], 0.0f,
                z[0], z[1], z[2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            if (!helios::math::transformTypeMatch(
                inheritance_, helios::math::TransformType::Rotation)) {
                /**
                 * @todo add support for different rotations
                 */
                setRotation(worldRotation);
                return;
            }

            // ParentTransform
            assert(parent() && "parent() of CameraSceneNode returned null, are you sure the node was added properly to the scenegraph?");
            const helios::math::mat4f pT = parent()->cachedWorldTransform();

            const helios::math::mat4f parentRotInv = pT.decompose(helios::math::TransformType::Rotation).transpose();

            // Apply rotation in local space by undoing the parent rotation
            setRotation(parentRotInv * worldRotation);
        }

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
        void lookAtLocal(helios::math::vec3f targetLocal, helios::math::vec3f upLocal) {
            // We treat up as a vector in world-space for now.
            const auto lt = localTransform().transform();
            const auto localPos = helios::math::vec3f(
                lt(0, 3), lt(1, 3), lt(2, 3)
            );

            auto z = (targetLocal - localPos).normalize();
            auto y = upLocal.normalize();
            auto x = helios::math::cross(y, z).normalize();
            y = helios::math::cross(z, x).normalize();

            const helios::math::mat4f localRotation = helios::math::mat4f{
                x[0], x[1], x[2], 0.0f,
                y[0], y[1], y[2], 0.0f,
                z[0], z[1], z[2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            };

            setRotation(localRotation);
        }

        /**
         * @brief Callback invoked when the world transform of this node changes.
         *
         * Overrides the base class implementation to update the camera's view matrix
         * whenever the node's position or orientation in the scene graph changes.
         * This ensures the camera always reflects the current world-space transform.
         *
         * @see SceneNode::onWorldTransformUpdate()
         */
        void onWorldTransformUpdate() noexcept override {

            helios::scene::SceneNode::onWorldTransformUpdate();

            // Updates this SceneNode's worldTransform_
            const auto wt = helios::scene::SceneNode::cachedWorldTransform();

            const auto x = helios::math::vec3f{wt(0, 0), wt(1, 0), wt(2, 0)};
            const auto y = helios::math::vec3f{wt(0, 1), wt(1, 1), wt(2, 1)};
            const auto z = helios::math::vec3f{wt(0, 2), wt(1, 2), wt(2, 2)};

            const auto eye = helios::math::vec3f{wt(0, 3), wt(1, 3), wt(2, 3)};

            // Compute view matrix: inverse of world transform with Z-negation for OpenGL RHS
            auto inv = helios::math::mat4f{
                x[0], y[0], -z[0], 0.0f,
                x[1], y[1], -z[1], 0.0f,
                x[2], y[2], -z[2], 0.0f,
                -dot(x, eye), -dot(y, eye), dot(z, eye), 1.0f
            };

            camera_->setViewMatrix(inv);
        }
    };

}
