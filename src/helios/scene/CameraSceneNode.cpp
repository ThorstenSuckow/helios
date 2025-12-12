module;

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

module helios.scene.CameraSceneNode;

import helios.math.types;
import helios.math.transform;
import helios.scene.SceneNode;
import helios.scene.InheritTransform;
import helios.scene.Camera;

namespace helios::scene {

    CameraSceneNode::CameraSceneNode(std::unique_ptr<Camera> camera) :
        camera_(std::move(camera)) {
        if (!camera_) {
            throw std::invalid_argument("CameraSceneNode received a Camera nullptr.");
        }
    }

    SceneNode* CameraSceneNode::addNode(std::unique_ptr<SceneNode> sceneNode) {
        assert(false && "CameraSceneNode does not accept child nodes.");
        return nullptr;
    }

    const Camera& CameraSceneNode::camera() const noexcept {
        return *camera_;
    }

    Camera& CameraSceneNode::camera() noexcept {
        return *camera_;
    }

    void CameraSceneNode::lookAtLocal(helios::math::vec3f targetLocal, helios::math::vec3f upLocal) {
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

        rotate(localRotation);
    }

    void CameraSceneNode::lookAt(helios::math::vec3f target, helios::math::vec3f up) {
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

        if (!helios::scene::InheritTransform::has(
            inheritance_, helios::scene::InheritTransform::Inherit::Rotation)) {
            /**
             * @todo add support for different rotations
             */
            rotate(worldRotation);
            return;
        }

        // ParentTransform
        assert(parent() && "parent() of CameraSceneNode returned null, are you sure the node was added properly to the scenegraph?");
        const helios::math::mat4f pT = parent()->cachedWorldTransform();


        // Basis-Spalten (column-major!)
        const helios::math::vec3f bx{ pT(0,0), pT(1,0), pT(2,0) }; // X-axis column
        const helios::math::vec3f by{ pT(0,1), pT(1,1), pT(2,1) }; // Y-axis column
        const helios::math::vec3f bz{ pT(0,2), pT(1,2), pT(2,2) }; // Z-axis column

        // Skalierung = Länge der Basisvektoren
        float sx = bx.length();
        float sy = by.length();
        float sz = bz.length();
        helios::math::vec3f parentScale{ sx, sy, sz };

        // Rotation ohne Scale (Basis normalisieren)
        helios::math::vec3f rx = (sx != 0.0f) ? helios::math::vec3f{ bx[0]/sx, bx[1]/sx, bx[2]/sx } : helios::math::vec3f{1,0,0};
        helios::math::vec3f ry = (sy != 0.0f) ? helios::math::vec3f{ by[0]/sy, by[1]/sy, by[2]/sy } : helios::math::vec3f{0,1,0};
        helios::math::vec3f rz = (sz != 0.0f) ? helios::math::vec3f{ bz[0]/sz, bz[1]/sz, bz[2]/sz } : helios::math::vec3f{0,0,1};

        // parentRot als mat4 (Spaltenweise!)
        const helios::math::mat4f parentRot{
            rx[0], rx[1], rx[2], 0.0f,
            ry[0], ry[1], ry[2], 0.0f,
            rz[0], rz[1], rz[2], 0.0f,
            0.0f,  0.0f,  0.0f,  1.0f
        };

        // Inverse der Rotation (weil orthonormal) = Transpose
        const helios::math::mat4f parentRotInv{
            parentRot(0,0), parentRot(1,0), parentRot(2,0), 0.0f,
            parentRot(0,1), parentRot(1,1), parentRot(2,1), 0.0f,
            parentRot(0,2), parentRot(1,2), parentRot(2,2), 0.0f,
            0.0f,          0.0f,          0.0f,          1.0f
        };

        // Apply rotation in local space by undoing the parent rotation
        rotate(parentRotInv * worldRotation);
    }

    void CameraSceneNode::onWorldTransformUpdate() noexcept {

        helios::scene::SceneNode::SceneNode::onWorldTransformUpdate();

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
        std::ignore = camera_->setViewMatrix(inv);
    }

} // namespace helios::scene
