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
import helios.rendering.RenderCommand;

import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPassFactory"
export namespace helios::rendering {

    /**
     * @brief A factory class for constructing `RenderPass` objects and populating
     * their `RenderQueue`s based on a `helios::scene::Snapshot`.
     *
     * @note This class implements a singleton pattern. In the future, the conversion of high-level
     * scene data (Snapshot/SnapshotItems) into lower-level rendering
     * commands might be realized with strategies and/or separate factories.
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
         * @brief Builds a `RenderPass` from a given `helios::scene::Snapshot`.
         *
         * This method orchestrates the creation of a `RenderQueue` and populates it
         * with `RenderCommand`s derived from the `SnapshotItem`s within the snapshot.
         * It also sets up frame-specific uniform values for the `RenderPass`.
         *
         * @param snapshot A const ref to the snapshot containing the scene data for which
         * the `RenderPass` is to be built.
         *
         * @return A fully constructed `RenderPass` object. The returned object owns the
         * `RenderQueue` and the frame-specific `UniformValueMap`.
         *
         * @see ::populateRenderQueue()
         */
        [[nodiscard]] RenderPass buildRenderPass(const helios::scene::Snapshot& snapshot) const {

            auto renderQueue = std::make_unique<RenderQueue>();

            populateRenderQueue(snapshot, *renderQueue);

            const auto& projectionMatrix = snapshot.projectionMatrix();
            const auto& viewMatrix = snapshot.viewMatrix();

            auto frameUniformValues = std::make_unique<helios::rendering::shader::UniformValueMap>();
            frameUniformValues->set(helios::rendering::shader::UniformSemantics::ProjectionMatrix, projectionMatrix);
            frameUniformValues->set(helios::rendering::shader::UniformSemantics::ViewMatrix, viewMatrix);

            return RenderPass(
                snapshot.viewport(),
                std::move(renderQueue),
                std::move(frameUniformValues)
            );
        }

        /**
         * @brief Populates an existing `RenderQueue` with `RenderCommand`s based on a `Snapshot`.
         *
         * This method clears the specified `RenderQueue` before adding new `RenderCommand`s to it.
         *
         * @param snapshot A const ref to the `Snapshot` providing scene data.
         * @param renderQueue A ref to the `RenderQueue` to be filled. This queue will be cleared before
         * new commands are added.
         *
         * @see ::makeRenderCommand()
         */
        void populateRenderQueue(
            const helios::scene::Snapshot& snapshot, helios::rendering::RenderQueue& renderQueue) const {

            // clear the queue
            renderQueue.clear();

            const auto& snapshotItems = snapshot.snapshotItems();

            for (const auto& item : snapshotItems) {
                auto renderCommand = makeRenderCommand(item);
                if (renderCommand) {
                    renderQueue.add(std::move(renderCommand));
                }
                // if renderCommand is nullptr, it's already logged in makeRenderCommand, so we skip it
            }
        }

        /**
         * @brief Creates a single `RenderCommand` from a `helios::scene::SnapshotItem`.
         *
         * This method extracts necessary data from the `SnapshotItem` and bundles them
         * into a `RenderCommand` object.
         *
         * @param snapshotItem A const ref to the `SnapshotItem` from which to create
         * the `RenderCommand`.
         *
         * @return A unique pointer to the newly created `RenderCommand`. Returns nullptr
         * if the associated Renderable is no longer available, or if the
         * shader is not available from the `Renderable`'s material, or if the mesh is
         * no longer available.
         *
         * @todo future versions might want to use a FactoryMethod in RenderCommand
         * that takes a SnapshotItem and creates a RenderCommand from it (e.g.
         * `RenderCommand::fromSnapshotItem()`); or a Factory `RenderCommandBuilder()`
         * with fluent configuration `.withShader().withMesh().build();`
         */
        [[nodiscard]] std::unique_ptr<RenderCommand> makeRenderCommand(
            const helios::scene::SnapshotItem& snapshotItem) const noexcept {

            const auto& renderable = snapshotItem.renderable();

            const auto sharedRenderable = renderable.lock();
            if (sharedRenderable == nullptr) {
                logger_.warn("Renderable no longer available");
                return nullptr;
            }

            auto objectUniformValues = std::make_unique<helios::rendering::shader::UniformValueMap>();
            auto materialUniformValues = std::make_unique<helios::rendering::shader::UniformValueMap>();
            objectUniformValues->set(
                helios::rendering::shader::UniformSemantics::ModelMatrix,
                snapshotItem.worldMatrix()
            );

            // make sure Material writes its uniform values
            /**
             * @todo when batching is implemented, this could be refactored
             * out of this factory method
             */
            sharedRenderable->writeUniformValues(*materialUniformValues);

            const auto renderPrototype = sharedRenderable->renderPrototype();

            return std::make_unique<RenderCommand>(
                renderPrototype,
                std::move(objectUniformValues),
                std::move(materialUniformValues)
            );
        }

    };

}