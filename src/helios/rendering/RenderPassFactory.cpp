module;

#include <memory>
#include <ostream>

module helios.rendering.RenderPassFactory;

import helios.scene.Snapshot;
import helios.scene.SnapshotItem;

import helios.rendering.RenderPass;
import helios.rendering.RenderQueue;
import helios.rendering.RenderCommand;

import helios.rendering.shader.UniformSemantics;
import helios.rendering.shader.UniformValueMap;

#define HELIOS_LOG_SCOPE "helios::rendering::RenderPassFactory"
namespace helios::rendering {


    RenderPassFactory& RenderPassFactory::getInstance() {
        static RenderPassFactory instance;

        return instance;
    }

    RenderPass RenderPassFactory::buildRenderPass(const helios::scene::Snapshot& snapshot) const {

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


    void RenderPassFactory::populateRenderQueue(
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


    std::unique_ptr<RenderCommand> RenderPassFactory::makeRenderCommand(
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
