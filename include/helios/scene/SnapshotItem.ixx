/**
 * @brief Snapshot item bundling a Renderable reference and its world matrix.
 */
module;

#include <memory>

export module helios.scene.SnapshotItem;

import helios.rendering.Renderable;
import helios.math.types;

export namespace helios::scene {

    /**
     * @brief Structure for representing a snapshot item of a renderable's object
     * state at a specific point in time.
     *
     * The structure bundles a weak pointer to a Renderable and a pre-calculated
     * worldMatrix, serving as a lightweight DTO to be used with a `Snapshot`.
     *
     * A SnapshotItem captures the essential information to render an item
     * without the need to traverse the entire Scene Graph during the
     * rendering phase.
     * The worldMatrix is a copy of the SceneNode's worldMatrix at the time of this
     * creation, ensuring that its value remains constant, even if the transformation
     * of the SceneNode should change during rendering.
     *
     * Note that for the Renderable a weak pointer is stored to allow for checking its
     * existence in the render pass. While internal properties of the Renderable
     * might change while this SnapshotItem is processed, the worldMatrix is immutable
     * and reflects the transformation state at the moment the Snapshot was taken.
     *
     * Thus, as a precondition, we assume that no changes to the Renderable are
     * made while the Renderable "exists" as a weak ptr in a particular Snapshot
     * of a scene that would lead to invalidating its rendering. We rely on the
     * state information available within the Renderable as a reflection of its
     * state when the Snapshot was taken.
     *
     * @todo we're intentionally preferring stability over performance by using
     * weak_ptr instead of raw pointers. This leaves room for (performance) improvements
     * in later iterations.
     * The Renderable as a weak pointer can later on be further implemented into
     * a pure DTO, storing only scalar values that get send directly to the underlying
     * rendering backend (e.g. vaos, vbos etc.).
     */
    struct SnapshotItem {
    private:
        /**
         * @brief A weak pointer to the actual Renderable object.
         */
        std::weak_ptr<const helios::rendering::Renderable> renderable_;

        /**
         * @brief A copy of the world transformation matrix for the Renderable.
         */
        const helios::math::mat4f worldMatrix_;

    public:
        /**
         * @brief Prevent copying.
         */
        SnapshotItem(const SnapshotItem&)=delete;
        SnapshotItem& operator=(const SnapshotItem&)=delete;

        /**
         * @brief Provide custom move constructor
         * @see https://en.cppreference.com/w/cpp/language/move_constructor.html
         */
        SnapshotItem(SnapshotItem&&) noexcept = default;

        /**
         * @brief Constructs a new SnapshotItem with the specified renderable
         * and the worldMatrix.
         *
         * @param renderable A weak_ptr to the Renderable.
         * @param worldMatrix A const ref to the world transformation matrix, which gets copied.
         */
        SnapshotItem(
            std::weak_ptr<const helios::rendering::Renderable> renderable,
            const helios::math::mat4f& worldMatrix
        );

        /**
         * @brief Returns a weak_ptr to the Renderable of this SnapshotItem.
         * Use `.lock()` on the returned weak_ptr to obtain a shared_ptr for
         * the Renderable, if it still exists. If the Renderable does not exist,
         * the SnapshotItem should be ignored for further processing.
         *
         * @return A weak_ptr to the Renderable of this SnapshotItem.
         */
        [[nodiscard]] std::weak_ptr<const helios::rendering::Renderable> renderable() const noexcept;

        /**
         * @brief Returns a const ref to the worldMatrix of this SnapshotItem.
         * The world transformation matrix is a copy from the world transformation matrix
         * created with this SnapshotItem.
         *
         * @return The const ref to the world matrix of this SnapshotItem.
         */
        [[nodiscard]] const helios::math::mat4f& worldMatrix() const noexcept;
    };

}