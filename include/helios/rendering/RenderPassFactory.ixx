module;

#include <memory>
#include <ostream>

export module helios.rendering.RenderPassFactory;

import helios.scene.Snapshot;
import helios.scene.SnapshotItem;

import helios.rendering.RenderPass;
import helios.rendering.RenderQueue;
import helios.rendering.RenderCommand;

import helios.rendering.shader.UniformValueMap;

import helios.util.log.Logger;
import helios.util.log.LogManager;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPassFactory"
export namespace helios::rendering {

    /**
     * A factory class for constructing `RenderPass` objects and populate
     * their `RenderQueue`s based on a `helios::scene::Snapshot`.
     *
     * @note This class implements a singleton pattern. The conversion of high-level
     * scene data (Snapshot/SnapshotItems) into rather low level rendering
     * commands might be realized with strategies and/or separate factories in the future.
     */
    class RenderPassFactory {

    protected:
        /**
         * The logger used with this SRenderPassFactory.
         * Defaults to HELIOS_LOG_SCOPE
         *
         * @todo constructor injection
         */
        const helios::util::log::Logger& logger_ = helios::util::log::LogManager::getInstance().registerLogger(
            HELIOS_LOG_SCOPE
        );

    public:
        /**
         * Returns the singleton instance of `RenderPassFactory`.
         *
         * @return A reference to the single `RenderPassFactory` instance.
         */
        static RenderPassFactory& getInstance();

        /**
         * Builds a `RenderPass` from a given `helios::scene::Snapshot`.
         *
         * This method orchestrates the creation of a `RenderQueue` and populates it
         * with `RenderCommand`s derived from the `SnaphotItem`s within the snapshot.
         * It also takes care of setting up frame-specific uniform values for the
         * `RenderPass`.
         *
         * @param snapshot A const ref to the snapshot containing the scene data for which
         * the `RenderPass` is to be built.
         *
         * @return A fully constructed `RenderPass` object. The `RenderPass` takes ownership of the
         * `RenderQueue` and the frame specific `UniformValueMap`.
         *
         * @see ::populateRenderQueue()
         */
        [[nodiscard]] RenderPass buildRenderPass(const helios::scene::Snapshot& snapshot) const;

        /**
         * Populates an existing `RenderQueue` with `RenderCommand`s based on a `Snapshot`.
         *
         * This method clears the specified `RenderQueue` before adding new `RenderCommand`s to it.
         *
         * @param snapshot A const ref to the `Snapshot` providing scene data.
         * @param renderQueue A ref to the `RenderQueue` to be filled. This queue will be cleared before
         * before new commands are added.
         *
         * @see ::makeRenderCommand()
         */
        void populateRenderQueue(
            const helios::scene::Snapshot& snapshot, helios::rendering::RenderQueue& renderQueue) const;

        /**
         * Creates a single `RenderCommand` from a `helios::scene::SnapshotItem`.
         *
         * This method extracts necessary data from the `SnapshotItem` and bundles them
         * into a `RenderCommand` object.
         *
         * @param snapshotItem A const ref to the `SnapshotItem` from which to create
         * the `RenderCommand`.
         *
         * @return A unique pointer to the newly created `RenderCommand`. Returns nullptr
         * if the associated rendabled is no longer available, or if the
         * shader is not available from the `Renderable`'s material, or if the mesh is
         * no longer available.
         *
         * @todo future versions might want to use a FactoryMethod in RenderCommand
         * that takes a SnapshotItem and creates a RenderCommand from it (e.g.
         * RenderCommand::fromSnapshotItem()); or a Factory ReanderCommandBuilder()
         * .withShader().withMesh().build();
         */
        [[nodiscard]] std::unique_ptr<RenderCommand> makeRenderCommand(
            const helios::scene::SnapshotItem& snapshotItem) const noexcept;

    };

}