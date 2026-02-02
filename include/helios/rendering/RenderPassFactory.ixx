/**
 * @file RenderPassFactory.ixx
 * @brief Factory for creating RenderPass objects from scene snapshots.
 */
module;

#include <memory>
#include <ostream>

export module helios.rendering.RenderPassFactory;

import helios.scene.Snapshot;
import helios.scene.SnapshotItem;

import helios.rendering.RenderPass;
import helios.rendering.RenderQueue;

import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPassFactory"
export namespace helios::rendering {

    /**
     * @brief Factory for constructing `RenderPass` objects from scene snapshots.
     *
     * `RenderPassFactory` transforms high-level scene data (`Snapshot` and `SnapshotItem`)
     * into low-level rendering commands suitable for the rendering device. It creates
     * `RenderPass` objects with populated `RenderQueue`s and configured frame uniforms.
     *
     * ## Responsibilities
     *
     * - Build `RenderPass` objects from `Snapshot` data.
     * - Populate `RenderQueue` with `MeshRenderCommand` and `TextRenderCommand` objects.
     * - Configure frame-level uniforms (view/projection matrices).
     *
     * ## Usage
     *
     * ```cpp
     * auto& factory = RenderPassFactory::getInstance();
     * auto snapshot = sceneGraph.createSnapshot(viewport);
     * auto renderPass = factory.buildRenderPass(snapshot);
     *
     * device.beginRenderPass(renderPass);
     * device.doRender(renderPass);
     * device.endRenderPass(renderPass);
     * ```
     *
     * @note This class implements a singleton pattern. In the future, the conversion
     *       of high-level scene data into lower-level rendering commands might be
     *       realized with strategies and/or separate factories.
     *
     * @see RenderPass
     * @see Snapshot
     * @see SnapshotItem
     * @see RenderQueue
     */
    class RenderPassFactory {

    protected:
        /**
         * @brief Shared logger instance for all RenderPassFactory objects.
         */
        inline static const helios::util::log::Logger& logger_ = helios::util::log::LogManager::loggerForScope(
            HELIOS_LOG_SCOPE
        );

    public:
        /**
         * @brief Returns the singleton instance of `RenderPassFactory`.
         *
         * @return A reference to the single `RenderPassFactory` instance.
         */
        static RenderPassFactory& getInstance() {
            static RenderPassFactory instance;

            return instance;
        }

        /**
         * @brief Builds a `RenderPass` from a given `Snapshot`.
         *
         * This method orchestrates the creation of a `RenderQueue` and populates it
         * with render commands derived from the `SnapshotItem`s within the snapshot.
         * It also sets up frame-specific uniform values (view and projection matrices)
         * for the `RenderPass`.
         *
         * @param snapshot A const reference to the snapshot containing the scene data
         *                 for which the `RenderPass` is to be built.
         *
         * @return A fully constructed `RenderPass` object containing the render queue
         *         and frame-specific uniforms.
         *
         * @see populateRenderQueue()
         */
        [[nodiscard]] RenderPass buildRenderPass(const helios::scene::Snapshot& snapshot) const {

            auto renderQueue = RenderQueue();

            populateRenderQueue(snapshot, renderQueue);

            const auto& projectionMatrix = snapshot.projectionMatrix();
            const auto& viewMatrix = snapshot.viewMatrix();

            auto frameUniformValues = helios::rendering::shader::UniformValueMap();
            frameUniformValues.set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, projectionMatrix);
            frameUniformValues.set(helios::rendering::shader::UniformSemantics::ViewMatrix, viewMatrix);

            return RenderPass(
                &snapshot.viewport(),
                std::move(renderQueue),
                frameUniformValues
            );
        }

        /**
         * @brief Populates an existing `RenderQueue` with render commands based on a `Snapshot`.
         *
         * This method clears the specified `RenderQueue` before adding new commands.
         * Each `SnapshotItem` in the snapshot is processed via `makeRenderCommand()`.
         *
         * @param snapshot A const reference to the `Snapshot` providing scene data.
         * @param renderQueue A reference to the `RenderQueue` to be filled. This queue
         *                    will be cleared before new commands are added.
         *
         * @see makeRenderCommand()
         */
        void populateRenderQueue(
            const helios::scene::Snapshot& snapshot, helios::rendering::RenderQueue& renderQueue) const {

            // clear the queue
            renderQueue.clear();

            auto snapshotItems = snapshot.snapshotItems();

            for (const auto& item : snapshotItems) {
                makeRenderCommand(item, renderQueue);
            }
        }

        /**
         * @brief Emits render commands from a `SnapshotItem` to the render queue.
         *
         * This method extracts necessary data from the `SnapshotItem`, sets up the object
         * uniform values (e.g., model matrix), and delegates to the `Renderable` to emit
         * its render commands to the queue.
         *
         * @param snapshotItem A const reference to the `SnapshotItem` from which to create
         *                     render commands.
         * @param renderQueue A reference to the `RenderQueue` to emit commands to.
         *
         * @note If the `Renderable` pointer in the `SnapshotItem` is `nullptr`, a warning
         *       is logged and no commands are emitted.
         */
        void makeRenderCommand(
            const helios::scene::SnapshotItem& snapshotItem,
             helios::rendering::RenderQueue& renderQueue) const noexcept {

            const auto* renderable = snapshotItem.renderable();

            if (renderable == nullptr) {
                logger_.warn("Renderable no longer available");
                return;
            }

            auto objectUniformValues = helios::rendering::shader::UniformValueMap();
            auto materialUniformValues = helios::rendering::shader::UniformValueMap();
            objectUniformValues.set(
                helios::rendering::shader::UniformSemantics::ModelMatrix,
                snapshotItem.worldMatrix()
            );

            renderable->emit(
                renderQueue,
               objectUniformValues,
                materialUniformValues
            );

        }

    };

}