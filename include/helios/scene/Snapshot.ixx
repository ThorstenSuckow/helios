/**
 * @file Snapshot.ixx
 * @brief Represents an immutable snapshot of a SceneGraph at a given point in time.
 */
module;


#include <memory>
#include <span>
#include <vector>

export module helios.scene.Snapshot;

import helios.rendering.Viewport;
import helios.scene.SnapshotItem;
import helios.math.types;

export namespace helios::scene {

    /**
     * @brief Represents an immutable snapshot of a SceneGraph at a given point in time.
     *
     * The class encapsulates all necessary data to render a single frame. It is
     * designed to be a lightweight DTO to pass information to the rendering
     * engine abstraction layer, such as the `RenderPass` class.
     *
     * A snapshot includes a camera's projection and view matrix that should be used
     * for rendering the scene. A collection of `SnapshotItems` holds the information
     * about the renderable objects in the scene, preferably computed by using a
     * `FrustumCullingStrategy` (@see Scene::createSnapshot).
     *
     * The class is not copyable to enforce its immutability and prevent unintended
     * duplication of potentially large data sets.
     */
    class Snapshot final {

    protected:
        /**
         * @brief The projection matrix that should be used with this Snapshot.
         *
         * The projection matrix transforms camera space into clip space.
         */
        const helios::math::mat4f projectionMatrix_;

        /**
         * @brief The view matrix that should be used with this Snapshot.
         *
         * The view matrix transforms world space coordinates into camera space.
         */
        const helios::math::mat4f viewMatrix_;

        /**
         * @brief A const collection of `SnapshotItem`s representing all renderable objects visible in this Snapshot.
         *
         * Each item contains the world transformation along with the renderable object.
         */
        const std::vector<SnapshotItem> snapshotItems_;

        /**
         * @brief The viewport associated with this Snapshot.
         */
        const helios::rendering::Viewport& viewport_;

    public:

        ~Snapshot() = default;

        /**
         * @brief Delete copy constructor.
         *
         * Snapshots are not intended to be copied.
         */
        Snapshot(const Snapshot&) = delete;

        /**
         * @brief Delete copy assignment operator.
         *
         * Snapshots are not intended to be copied.
         */
        Snapshot& operator=(const Snapshot&) = delete;

        /**
         * @brief Delete move constructor.
         */
        Snapshot(Snapshot&&) noexcept = delete;

        /**
         * @brief Delete move assignment.
         */
        Snapshot& operator=(Snapshot&&) noexcept = delete;

        /**
         * @brief Constructs a new immutable Snapshot from the given data.
         *
         * @param viewport The viewport associated with this snapshot.
         * @param projectionMatrix The projection matrix of the camera for this snapshot.
         * @param viewMatrix The view matrix of the camera for this snapshot.
         * @param snapshotItems A vector of snapshot items. The vector is moved into the Snapshot,
         * transferring ownership to this instance.
         */
        Snapshot(
            const helios::rendering::Viewport& viewport,
            const math::mat4f& projectionMatrix,
            const math::mat4f& viewMatrix,
            std::vector<SnapshotItem> snapshotItems) noexcept :
            viewport_(viewport),
            projectionMatrix_(projectionMatrix),
            viewMatrix_(viewMatrix),
            snapshotItems_(std::move(snapshotItems))
        {}

        /**
         * @brief Returns a const reference to this Snapshot's SnapshotItems.
         *
         * The vector contains all renderable items in the scene.
         *
         * @return A const reference to this Snapshot's collection of SnapshotItems.
         */
        [[nodiscard]] std::span<const SnapshotItem> snapshotItems() const noexcept {
            return snapshotItems_;
        }

        /**
         * @brief Returns the viewport associated with this Snapshot.
         *
         * @return A shared pointer to the const Viewport.
         */
        [[nodiscard]] const helios::rendering::Viewport& viewport() const noexcept {
            return viewport_;
        }

        /**
         * @brief Returns a const reference to the projection matrix for this Snapshot.
         *
         * The matrix represents the projection matrix of the camera that was used to
         * capture this scene.
         *
         * @return A const reference to the projection matrix used for creating this Snapshot.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() const noexcept {
            return projectionMatrix_;
        }

        /**
         * @brief Returns a const reference to the view matrix for this Snapshot.
         *
         * The matrix represents the view matrix of the camera that was used to
         * capture this scene.
         *
         * @return A const reference to the view matrix used for creating this Snapshot.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() const noexcept {
            return viewMatrix_;
        }
    };
}

