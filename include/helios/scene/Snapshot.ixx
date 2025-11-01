module;

#include <memory>
#include <vector>

export module helios.scene.Snapshot;

import helios.scene.SnapshotItem;
import helios.math.types;

export namespace helios::scene {

    /**
     * @brief Represents an immutable snapshot of a SceneGraph at a given point in time.
     *
     * The class encapsulated all necessary data to render a single frame. It is
     * designed to be a lightweight dto to pass information to the rendering
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
         * The projection matrix transforms camera space into clip space.
         */
        const helios::math::mat4f projectionMatrix_;

        /**
         * @brief The view matrix that should be used with this Snapshot.
         * The view matrix transforms world space coordinates into camera space.
         */
        const helios::math::mat4f viewMatrix_;

        /**
         * @brief A const collection of `SnapshotItem`s representing all renderable objects along with
         * their world transformation visible in this Snapshot.
         */
        const std::vector<SnapshotItem> snapshotItems_;


    public:

        ~Snapshot() = default;

        /**
         * @brief Delete copy constructor.
         * Snapshots are not intended to by copied.
         */
        Snapshot(const Snapshot&) = delete;

        /**
         * @brief Delete copy assignment operator.
         * Snapshots are not intended tp be copied.
         */
        Snapshot& operator=(const Snapshot&) = delete;

        /**
         * @brief Constructs a new immutable Snapshot from the given data.
         *
         * @param projectionMatrix The projection matrix of the camera for this snapshot.
         * @param viewMatrix The view matrix of the camera for this snapshot.
         * @param snapshotItems A std::vector of snapshot items. The vector is moved into the Snapshot,
         * transferring ownership to **this** instance.
         */
        Snapshot(
            const math::mat4f& projectionMatrix,
            const math::mat4f& viewMatrix,
            std::vector<SnapshotItem> snapshotItems) noexcept;

        /**
         * @brief Returns a const ref to this `Snapshot`'s SnapshotItems.
         * The vector contains all renderable items in the scene.
         *
         * @return A const ref to this Snapshot's colelction of SnapshotItems.
         */
        [[nodiscard]] const std::vector<SnapshotItem>& snapshotItems() const noexcept;

        /**
         * @brief Returns a const ref to the projection matrix for this Snapshot.
         * The matrix represents the projection matrix of the camera that was used to
         * capture this scene.
         * @return A const ref to the projection matrix used for creating this Snapshot.
         */
        [[nodiscard]] const helios::math::mat4f& projectionMatrix() const noexcept;

        /**
         * @brief Returns a const ref to the view matrix for this Snapshot.
         * The matrix represents the view matrix of the camera that was used to
         * capture this scene.
         * @return A const ref to the view matrix used for creating this Snapshot.
         */
        [[nodiscard]] const helios::math::mat4f& viewMatrix() const noexcept;
    };
}

