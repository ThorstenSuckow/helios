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

        return RenderPass(std::move(renderQueue), std::move(frameUniformValues));
    }


    void RenderPassFactory::populateRenderQueue(
        const helios::scene::Snapshot& snapshot, helios::rendering::RenderQueue& renderQueue) const {

        // clear the queue
        renderQueue.clear();

        const auto& snapshotItems = snapshot.snapshotItems();

        for (const auto& item : snapshotItems) {
            renderQueue.add(makeRenderCommand(item));
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
        objectUniformValues->set(helios::rendering::shader::UniformSemantics::WorldMatrix, snapshotItem.worldMatrix());

        // make sure Material writes its uniform values
        sharedRenderable->material().writeUniformValues(*materialUniformValues);

        const auto shader = sharedRenderable->material().shader();
        if (!shader) {
            logger_.warn("Shader no longer available");
            return nullptr;
        }

        const auto& mesh = sharedRenderable->mesh();
        if (!mesh) {
            logger_.warn("Mesh no longer available");
            return nullptr;
        }

        return std::make_unique<RenderCommand>(
            shader,
            mesh,
            std::move(objectUniformValues),
            std::move(materialUniformValues)
        );
    }



};
