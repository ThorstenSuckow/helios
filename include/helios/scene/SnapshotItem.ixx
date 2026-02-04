/**
 * @file SnapshotItem.ixx
 * @brief Snapshot item bundling a Renderable reference and its world matrix.
 */
module;

#include <cassert>
#include <memory>

export module helios.scene.SnapshotItem;

import helios.rendering.Renderable;
import helios.math.types;

export namespace helios::scene {

    /**
     * @brief Structure for representing a snapshot item of a renderable's object
     * state at a specific point in time.
     *
     * The structure bundles a non-owning raw pointer to a Renderable and a pre-calculated
     * worldMatrix, serving as a lightweight DTO to be used with a `Snapshot`.
     *
     * A SnapshotItem captures the essential information to render an item
     * without the need to traverse the entire Scene Graph during the
     * rendering phase.
     * The worldMatrix is a copy of the SceneNode's worldMatrix at the time of this
     * creation, ensuring that its value remains constant, even if the transformation
     * of the SceneNode should change during rendering.
     *
     * The caller must ensure that the Renderable remains valid for the lifetime
     * of this SnapshotItem (typically within a single frame).
     *
     * While internal properties of the Renderable might change while this SnapshotItem
     * is processed, the worldMatrix is immutable and reflects the transformation state
     * at the moment the Snapshot was taken.
     */
    struct SnapshotItem {
    private:
        /**
         * @brief A non-owning raw pointer to the Renderable object.
         *
         * The Renderable must remain valid for the lifetime of this SnapshotItem.
         */
        const helios::rendering::Renderable* renderable_;

        /**
         * @brief A copy of the world transformation matrix for the Renderable.
         */
        const helios::math::mat4f worldMatrix_;

    public:


        /**
         * @brief Constructs a new SnapshotItem with the specified renderable
         * and the worldMatrix.
         *
         * @param renderable A raw pointer to the Renderable (must remain valid for the lifetime of this item).
         * @param worldMatrix A const ref to the world transformation matrix, which gets copied.
         */
        SnapshotItem(
            const helios::rendering::Renderable* renderable,
            const helios::math::mat4f& worldMatrix
        ) :
            renderable_(renderable),
            worldMatrix_(worldMatrix) {
            assert(renderable_ && "Unexpected nullptr forRenderable");
        }

        /**
         * @brief Returns a raw pointer to the Renderable of this SnapshotItem.
         *
         * @return A raw pointer to the Renderable of this SnapshotItem.
         */
        [[nodiscard]] const helios::rendering::Renderable* renderable() const noexcept {
            return renderable_;
        }

        /**
         * @brief Returns a const ref to the worldMatrix of this SnapshotItem.
         * The world transformation matrix is a copy from the world transformation matrix
         * created with this SnapshotItem.
         *
         * @return The const ref to the world matrix of this SnapshotItem.
         */
        [[nodiscard]] const helios::math::mat4f& worldMatrix() const noexcept {
            return worldMatrix_;
        }
    };

}